#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec4 u_viewport;
uniform float u_pixel_width;

noperspective out vec2 v_position;
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

// We draw quad at current point
void build_quad(vec4 curr)
{
	vec2 screen_curr = project(curr);

	float w = u_pixel_width * 0.5;

	// Quad
	gl_Position = unproject(screen_curr + vec2(-w, w), curr.z, curr.w); // left top
	v_position = vec2(-w, w);
	v_radius = w;
	EmitVertex();
	gl_Position = unproject(screen_curr + vec2(-w, -w), curr.z, curr.w); // left bottom
	v_position = vec2(-w, -w);
	v_radius = w;
	EmitVertex();
	gl_Position = unproject(screen_curr + vec2(w, w), curr.z, curr.w); // right top
	v_position = vec2(w, w);
	v_radius = w;
	EmitVertex();   
	gl_Position = unproject(screen_curr + vec2(w, -w), curr.z, curr.w); // right bottom
	v_position = vec2(w, -w);
	v_radius = w;
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_quad(gl_in[0].gl_Position);
}