#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	float size;
	vec4 color;
} gs_in[];

flat out vec4 v_color;

void build_square(vec4 pos)
{
	float size = gs_in[0].size;
	vec4 color = gs_in[0].color;

	gl_Position = vec4(pos.x - size, pos.y + size, pos.z, pos.w); // left top
	v_color = color;
	EmitVertex();   
	gl_Position = vec4(pos.x - size, pos.y - size, pos.z, pos.w); // left bottom
	v_color = color;
	EmitVertex();
	gl_Position = vec4(pos.x + size, pos.y + size, pos.z, pos.w); // right top
	v_color = color;
	EmitVertex();   
	gl_Position = vec4(pos.x + size, pos.y - size, pos.z, pos.w); // right bottom
	v_color = color;
	EmitVertex();
	EndPrimitive();
}

void main()
{    
	build_square(gl_in[0].gl_Position);
}