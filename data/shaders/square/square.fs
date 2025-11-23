#version 330 core

noperspective in vec2 v_position;
flat in vec4 v_color;

out vec4 out_color;

void main()
{
	vec2 position = v_position;
	float alpha = max(1.0 - length(position), 0.0);
	out_color = vec4(alpha);
}