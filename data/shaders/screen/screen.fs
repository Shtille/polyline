#version 330 core

uniform sampler2D u_texture; // alpha texture
uniform vec4 u_background_color;
uniform vec4 u_object_color;

noperspective in vec2 v_texcoord;

out vec4 color;

void main()
{
	// Classic blending formula is:
	// color = src_color * src_alpha + dst_color * (1 - src_alpha)
	// mix(x, y, a) = x * (1-a) + y * a
	vec4 sample = texture(u_texture, v_texcoord);
	float alpha = sample.x;
	color = mix(u_background_color, u_object_color, alpha);
}