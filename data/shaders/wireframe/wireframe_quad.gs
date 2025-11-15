#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec4 u_viewport;
uniform float u_hard_radius;
uniform float u_soft_radius;

noperspective out vec2 v_position;

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
void build_quad(vec4 curr, vec4 next)
{
	vec2 screen_curr = project(curr);
	vec2 screen_next = project(next);

	vec2 d2 = screen_next - screen_curr;
	float segment_length = length(d2);
	d2 /= segment_length; // normalize
	vec2 n2 = vec2(-d2.y, d2.x);

	float w = u_hard_radius + u_soft_radius;

	vec2 offset_y = n2 * w;
	float cap_offset_curr = 0.0;
	float cap_offset_next = 0.0;
	vec2 curr_offset_x = d2 * cap_offset_curr;
	vec2 next_offset_x = d2 * cap_offset_next;

	// Quad
	gl_Position = unproject(screen_curr + offset_y + curr_offset_x, curr.z, curr.w); // left top
	v_position = vec2(cap_offset_curr, w);
	EmitVertex();
	gl_Position = unproject(screen_curr - offset_y + curr_offset_x, curr.z, curr.w); // left bottom
	v_position = vec2(cap_offset_curr, -w);
	EmitVertex();
	gl_Position = unproject(screen_next + offset_y + next_offset_x, next.z, next.w); // right top
	v_position = vec2(segment_length + cap_offset_next, w);
	EmitVertex();   
	gl_Position = unproject(screen_next - offset_y + next_offset_x, next.z, next.w); // right bottom
	v_position = vec2(segment_length + cap_offset_next, -w);
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_quad(gl_in[0].gl_Position, gl_in[1].gl_Position);
}