#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec4 u_viewport;
uniform float u_pixel_width;

in VS_OUT {
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

void build_quad(vec4 current, vec4 next)
{
	vec2 screen_current = project(current);
	vec2 screen_next = project(next);

	vec2 direction = normalize(screen_next - screen_current);
	vec2 normal = vec2(-direction.y, direction.x);

	float w = u_pixel_width * 0.5;

	gl_Position = unproject(screen_current + normal * w, current.z, current.w); // left top
	EmitVertex();   
	gl_Position = unproject(screen_current - normal * w, current.z, current.w); // left bottom
	EmitVertex();
	gl_Position = unproject(screen_next + normal * w, next.z, next.w); // right top
	EmitVertex();   
	gl_Position = unproject(screen_next - normal * w, next.z, next.w); // right bottom
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_quad(gl_in[0].gl_Position, gs_in[0].position_next);
}