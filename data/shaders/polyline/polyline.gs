#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

uniform vec4 u_viewport;
uniform float u_pixel_width;

in VS_OUT {
	vec4 position_prev;
	vec4 position_next;
} gs_in[];

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
	vec2 d2 = normalize(screen_next - screen_curr);
	vec2 n2 = vec2(-d2.y, d2.x);

	float w = u_pixel_width * 0.5;

	// Miter quad
	float cos_a = dot(-d1, d2);
	float miter_limit = 3.0 * w;
	float miter_distance = w * sqrt((1.0 + cos_a) / (1.0 - cos_a));
	if (cos_a < 0.98 && miter_distance < miter_limit)
	{
		float signed_z = w * sign(d1.x*d2.y - d2.x*d1.y);
		vec2 first_point  = screen_curr - n1 * signed_z;
		vec2 second_point = screen_curr - n2 * signed_z;
		vec2 miter_point  = first_point + d1 * miter_distance;
		gl_Position = unproject(first_point, curr.z, curr.w);
		EmitVertex();
		gl_Position = curr;
		EmitVertex();
		gl_Position = unproject(miter_point, curr.z, curr.w);
		EmitVertex();
		gl_Position = unproject(second_point, curr.z, curr.w);
		EmitVertex();
		EndPrimitive();
	}

	// // Bevel triangle
	// float signed_z = w * sign(d1.x*d2.y - d2.x*d1.y);
	// gl_Position = unproject(screen_curr - n1 * signed_z, curr.z, curr.w);
	// EmitVertex();
	// gl_Position = unproject(screen_curr - n2 * signed_z, curr.z, curr.w);
	// EmitVertex();
	// gl_Position = curr;
	// EmitVertex();
	// EndPrimitive();

	// Quad
	gl_Position = unproject(screen_curr + n2 * w, curr.z, curr.w); // left top
	EmitVertex();
	gl_Position = unproject(screen_curr - n2 * w, curr.z, curr.w); // left bottom
	EmitVertex();
	gl_Position = unproject(screen_next + n2 * w, next.z, next.w); // right top
	EmitVertex();   
	gl_Position = unproject(screen_next - n2 * w, next.z, next.w); // right bottom
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_quad(gs_in[0].position_prev, gl_in[0].gl_Position, gs_in[0].position_next);
}