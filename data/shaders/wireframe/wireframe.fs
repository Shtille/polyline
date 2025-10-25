#version 330 core

uniform vec4 u_color;

out vec4 color;

noperspective in vec2 v_position;
flat in float v_length;
flat in float v_radius;

void main()
{
	vec2 position = v_position;
	float len = v_length;
	float radius = v_radius;

	float d = abs(position.y);

	if (position.x < 0.0)
	{
		float dist = length(position); // = distance(position, vec2(0.0))
		d = dist;
	}
	else if (position.x > len)
	{
		float dist = length(vec2(position.x - len, position.y)); // = distance(position, vec2(len, 0.0))
		d = dist;
	}

	d += 1.0 - radius;
	if (d < 0.0)
		color = u_color;
	else
		color = vec4(u_color.rgb, u_color.a * exp(-d*d));
}