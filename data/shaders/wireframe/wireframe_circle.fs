#version 330 core

uniform vec4 u_color;
uniform float u_hard_radius;
uniform float u_soft_radius;

out vec4 color;

noperspective in vec2 v_position;

void main()
{
	vec2 position = v_position;

	float d = length(position);

	d -= u_hard_radius;
	if (d < 0.0)
		color = u_color;
	else
	{
		d /= u_soft_radius;
		float alpha = max(1.0 - d, 0.0);
		alpha *= alpha;
		color = vec4(u_color.rgb, u_color.a * alpha); // exp(-d*d)
	}
}