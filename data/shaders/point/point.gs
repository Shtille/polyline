#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float u_aspect_ratio;

void build_square(vec4 position)
{
	const float w = 0.1;
	float wx = w / u_aspect_ratio;
	float wy = w;
	gl_Position = position + vec4(-wx, -wy, 0.0, 0.0);
	EmitVertex();   
	gl_Position = position + vec4( wx, -wy, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-wx,  wy, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( wx,  wy, 0.0, 0.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_square(gl_in[0].gl_Position);
}