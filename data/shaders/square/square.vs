#version 330 core

layout (location = 0) in vec2  a_position;
layout (location = 1) in float a_size;
layout (location = 2) in vec4  a_color;

out VS_OUT {
	float size;
	vec4 color;
} vs_out;

void main()
{
	gl_Position = vec4(a_position, 0.0, 1.0);
	vs_out.size = a_size;
	vs_out.color = a_color;
}