#version 330 core

uniform sampler2D u_texture;

noperspective in vec2 v_texcoord;

out vec4 color;

void main()
{
	vec4 sample = texture(u_texture, v_texcoord);
	color = sample;
}