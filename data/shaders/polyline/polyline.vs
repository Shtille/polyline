#version 330 core

layout (location = 0) in vec2 a_position_prev;
layout (location = 1) in vec2 a_position_curr;
layout (location = 2) in vec2 a_position_next;
layout (location = 3) in float a_point_type_curr;
layout (location = 4) in float a_point_type_next;

out VS_OUT {
	vec4 position_prev;
	vec4 position_next;
	float point_type_curr;
	float point_type_next;
} vs_out;

void main()
{
	gl_Position = vec4(a_position_curr, 0.0, 1.0);
	vs_out.position_prev = vec4(a_position_prev, 0.0, 1.0);
	vs_out.position_next = vec4(a_position_next, 0.0, 1.0);
	vs_out.point_type_curr = a_point_type_curr;
	vs_out.point_type_next = a_point_type_next;
}