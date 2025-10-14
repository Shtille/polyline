#version 330 core

uniform int u_cap_style; // flat, square, round
uniform int u_join_style; // bevel, miter, round

out vec4 color;

noperspective in vec2 v_position;
noperspective in float v_point_type;
flat in float v_length;
flat in float v_radius;

void main()
{
	vec2 position = v_position;
	float point_type = v_point_type;
	float len = v_length;
	float radius = v_radius;

	bool is_join = abs(point_type) < 0.5;
	bool need_discard = !is_join && u_cap_style == 2 || is_join && u_join_style == 2;

	if (need_discard && position.x < 0.0)
	{
		float dist = length(position); // = distance(position, vec2(0.0))
		if (dist > radius)
			discard;
	}
	else if (need_discard && position.x > len)
	{
		float dist = length(vec2(position.x - len, position.y)); // = distance(position, vec2(len, 0.0))
		if (dist > radius)
			discard;
	}
	color = vec4(0.0, 1.0, 0.0, 1.0);
}