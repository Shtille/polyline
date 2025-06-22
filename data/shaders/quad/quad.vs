#version 330 core

layout (location = 0) in vec2 a_position_curr;
layout (location = 1) in vec2 a_position_next;

out VS_OUT {
	vec4 position_next;
} vs_out;

void main()
{
	gl_Position = vec4(a_position_curr, 0.0, 1.0);
	vs_out.position_next = vec4(a_position_next, 0.0, 1.0);
}