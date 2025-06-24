#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

uniform vec4 u_viewport;
uniform float u_pixel_width;
uniform float u_miter_limit;
uniform int u_cap_style; // flat, square, round
uniform int u_join_style; // bevel, miter, round

in VS_OUT {
	vec4 position_prev;
	vec4 position_next;
	float point_type_curr;
	float point_type_next;
	float distance_curr;
	float distance_next;
} gs_in[];

noperspective out vec2 v_position;
noperspective out float v_point_type;
flat out float v_length;
flat out float v_radius;

vec2 project(vec4 clip)
{
    vec3 ndc = clip.xyz / clip.w;
    vec2 screen = (ndc.xy * 0.5 + vec2(0.5)) * u_viewport.zw + u_viewport.xy;
    return screen;
}
vec4 unproject(vec2 screen, float z, float w)
{
    vec2 ndc = ((screen - u_viewport.xy) / u_viewport.zw - vec2(0.5)) * 2.0;
    return vec4(ndc.x * w, ndc.y * w, z, w);
}

// We draw quad from current point to next one
void build_quad(vec4 prev, vec4 curr, vec4 next)
{
	vec2 screen_prev = project(prev);
	vec2 screen_curr = project(curr);
	vec2 screen_next = project(next);

	vec2 d1 = normalize(screen_curr - screen_prev);
	vec2 n1 = vec2(-d1.y, d1.x);
	vec2 d2 = screen_next - screen_curr;
	float segment_length = length(d2);
	d2 /= segment_length; // normalize
	vec2 n2 = vec2(-d2.y, d2.x);

	float w = u_pixel_width * 0.5;
	float point_type_curr = gs_in[0].point_type_curr;
	float point_type_next = gs_in[0].point_type_next;

	if (abs(point_type_curr) < 0.5) // current point is join, not start point
	{
		if (u_join_style == 0) // bevel join
		{
			// Bevel triangle
			float signed_z = w * sign(d1.x*d2.y - d2.x*d1.y);
			gl_Position = unproject(screen_curr - n1 * signed_z, curr.z, curr.w);
			v_position = vec2(0.0);
			v_point_type = 0.0;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			gl_Position = unproject(screen_curr - n2 * signed_z, curr.z, curr.w);
			v_position = vec2(0.0);
			v_point_type = 0.0;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			gl_Position = curr;
			v_position = vec2(0.0);
			v_point_type = 0.0;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			EndPrimitive();
		}
		else if (u_join_style == 1) // miter join
		{
			// Miter quad
			float cos_a = dot(-d1, d2);
			float miter_limit = u_miter_limit * w;
			float miter_distance = w * sqrt((1.0 + cos_a) / (1.0 - cos_a));
			if (cos_a < 0.98 && miter_distance < miter_limit)
			{
				float signed_z = w * sign(d1.x*d2.y - d2.x*d1.y);
				vec2 first_point  = screen_curr - n1 * signed_z;
				vec2 second_point = screen_curr - n2 * signed_z;
				vec2 miter_point  = first_point + d1 * miter_distance;
				gl_Position = unproject(first_point, curr.z, curr.w);
				v_position = vec2(0.0);
				v_point_type = 0.0;
				v_length = segment_length;
				v_radius = w;
				EmitVertex();
				gl_Position = curr;
				v_position = vec2(0.0);
				v_point_type = 0.0;
				v_length = segment_length;
				v_radius = w;
				EmitVertex();
				gl_Position = unproject(miter_point, curr.z, curr.w);
				v_position = vec2(0.0);
				v_point_type = 0.0;
				v_length = segment_length;
				v_radius = w;
				EmitVertex();
				gl_Position = unproject(second_point, curr.z, curr.w);
				v_position = vec2(0.0);
				v_point_type = 0.0;
				v_length = segment_length;
				v_radius = w;
				EmitVertex();
				EndPrimitive();
			}
		}
		else // round join
		{
			vec2 p1 = screen_curr + n2 * w;
			vec2 p2 = p1 - d2 * w;
			vec2 p3 = screen_curr - n2 * w;
			vec2 p4 = p3 - d2 * w;

			gl_Position = unproject(p1, curr.z, curr.w);
			v_position = vec2(0.0, w);
			v_point_type = point_type_curr;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			gl_Position = unproject(p2, curr.z, curr.w);
			v_position = vec2(-w, w);
			v_point_type = point_type_curr;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			gl_Position = unproject(p3, curr.z, curr.w);
			v_position = vec2(0.0, -w);
			v_point_type = point_type_curr;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			gl_Position = unproject(p4, curr.z, curr.w);
			v_position = vec2(-w, -w);
			v_point_type = point_type_curr;
			v_length = segment_length;
			v_radius = w;
			EmitVertex();
			EndPrimitive();
		}
	}

	vec2 curr_offset_x = vec2(0.0);
	vec2 next_offset_x = vec2(0.0);
	vec2 offset_y = n2 * w;
	float cap_offset_curr = 0.0;
	float cap_offset_next = 0.0;
	if (u_cap_style != 0) // not flat cap (square cap or round cap)
	{
		cap_offset_curr = w * point_type_curr;
		cap_offset_next = w * point_type_next;
		curr_offset_x = d2 * cap_offset_curr;
		next_offset_x = d2 * cap_offset_next;
	}

	// Quad
	gl_Position = unproject(screen_curr + offset_y + curr_offset_x, curr.z, curr.w); // left top
	v_position = vec2(cap_offset_curr, w);
	v_point_type = point_type_curr;
	v_length = segment_length;
	v_radius = w;
	EmitVertex();
	gl_Position = unproject(screen_curr - offset_y + curr_offset_x, curr.z, curr.w); // left bottom
	v_position = vec2(cap_offset_curr, -w);
	v_point_type = point_type_curr;
	v_length = segment_length;
	v_radius = w;
	EmitVertex();
	gl_Position = unproject(screen_next + offset_y + next_offset_x, next.z, next.w); // right top
	v_position = vec2(segment_length + cap_offset_next, w);
	v_point_type = point_type_next;
	v_length = segment_length;
	v_radius = w;
	EmitVertex();   
	gl_Position = unproject(screen_next - offset_y + next_offset_x, next.z, next.w); // right bottom
	v_position = vec2(segment_length + cap_offset_next, -w);
	v_point_type = point_type_next;
	v_length = segment_length;
	v_radius = w;
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_quad(gs_in[0].position_prev, gl_in[0].gl_Position, gs_in[0].position_next);
}