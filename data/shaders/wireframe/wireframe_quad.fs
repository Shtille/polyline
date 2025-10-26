#version 330 core

uniform vec4 u_color;

out vec4 color;

noperspective in vec2 v_position;
flat in float v_radius;

void main()
{
	vec2 position = v_position;
	float radius = v_radius;

	float d = abs(position.y);

	d += 1.0 - radius;
	if (d < 0.0)
		color = u_color;
	else
		color = vec4(u_color.rgb, u_color.a * exp(-d*d));
}