#version 330 core

layout (location = 0) in vec2 a_position;

noperspective out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(a_position, 0.0, 1.0);
	v_texcoord = a_position * 0.5 + 0.5;
}