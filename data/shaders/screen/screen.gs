#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

noperspective out vec2 v_texcoord;

void main()
{    
	gl_Position = vec4(-1.0, 1.0, 0.0, 1.0); // left top
	v_texcoord = vec2(0.0, 1.0);
	EmitVertex();   
	gl_Position = vec4(-1.0, -1.0, 0.0, 1.0); // left bottom
	v_texcoord = vec2(0.0, 0.0);
	EmitVertex();
	gl_Position = vec4(1.0, 1.0, 0.0, 1.0); // right top
	v_texcoord = vec2(1.0, 1.0);
	EmitVertex();   
	gl_Position = vec4(1.0, -1.0, 0.0, 1.0); // right bottom
	v_texcoord = vec2(1.0, 0.0);
	EmitVertex();
	EndPrimitive();
}