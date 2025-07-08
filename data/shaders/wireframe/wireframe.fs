#version 330 core

out vec4 color;

noperspective in vec2 v_position;
flat in float v_length;
flat in float v_radius;

void main()
{
	vec2 position = v_position;
	float len = v_length;
	float radius = v_radius;

	if (position.x < 0.0)
	{
		float dist = length(position); // = distance(position, vec2(0.0))
		if (dist > radius)
			discard;
	}
	else if (position.x > len)
	{
		float dist = length(vec2(position.x - len, position.y)); // = distance(position, vec2(len, 0.0))
		if (dist > radius)
			discard;
	}

	color = vec4(1.0, 0.0, 0.0, 1.0);
}