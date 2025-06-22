#version 330 core

layout(location = 0) in vec2 a_prev;
layout(location = 1) in vec2 a_curr;
layout(location = 2) in vec2 a_next;
layout(location = 3) in vec2 a_texcoord;

uniform vec4 u_viewport;
uniform float u_width;

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

void main()
{
	vec4 clip_curr = vec4(a_curr, 0.0, 1.0);
	vec4 clip_prev = vec4(a_prev, 0.0, 1.0);
	vec4 clip_next = vec4(a_next, 0.0, 1.0);

	vec2 curr = project(clip_curr);
	vec2 prev = project(clip_prev);
	vec2 next = project(clip_next);

	vec2 direction = normalize(next - prev);
	vec2 normal = vec2(-direction.y, direction.x);

	float w = u_width * 0.5;
	vec2 offset_x = direction * (a_texcoord.x * w);
	vec2 offset_y = normal    * (a_texcoord.y * w);
	vec2 screen = curr + offset_x + offset_y;

	gl_Position = unproject(screen, clip_curr.z, clip_curr.w);
}
