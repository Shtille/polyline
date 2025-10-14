#version 330 core

uniform float u_dash_unit;
uniform float u_dash_period;
uniform float u_dash_phase;
uniform sampler1D u_texture;

out vec4 color;

noperspective in vec2 v_position;
flat in float v_length;
flat in float v_radius;

float cap( float u, float v)
{
	return sqrt(u*u+v*v);
}

float div(float y, float x)
{
	return floor(y / x);
}

float distance_to_texcoord(float distance)
{
	return mod(distance + u_dash_phase, u_dash_period) / u_dash_period;
}
float dash_units_to_distance(float n, float units)
{
	return n * u_dash_period + units * u_dash_unit - u_dash_phase;
}

void main()
{
	vec2 position = v_position;
	float len = v_length;
	float radius = v_radius;
	
	float u = position.x;
	float v = position.y;

	float n = div(u + u_dash_phase, u_dash_period);

	// Transform pixel distance to range [0,1]
	float texcoord_x = distance_to_texcoord(u);
	vec4 dash = texture(u_texture, texcoord_x);
	// ref, start and stop are measured in dash units
	float dash_ref   = dash_units_to_distance(n, dash.x);
	float dash_type  = dash.y;
	float dash_start = dash_units_to_distance(n, dash.z);
	float dash_stop  = dash_units_to_distance(n, dash.w);
	float line_start = 0.0;
	float line_stop = len;

	bool cross_start = (dash_start <= line_start) && (dash_stop >= line_start);
	bool cross_stop = (dash_stop >= line_stop) && (dash_start <= line_stop);

	float d = abs(v);
	// Dash stop is before line start
	if (dash_stop <= line_start)
		discard;
	// Dash start is beyond line stop
	else if (dash_start >= line_stop)
		discard;
	// Dash is across line start and fragment before line start (1)
	else if (u <= line_start && cross_start)
		d = cap(u, v);
	// Dash is across line stop and fragment after line stop (4)
	else if (u >= line_stop && cross_stop)
		d = cap(u - line_stop, v);
	// Dash cap start (5)
	else if (dash_type < 0.0) // -1
		d = cap(u - dash_ref, v);
	// Dash cap stop (6)
	else if (dash_type > 0.0) // 1
		d = cap(dash_ref - u, v);

	if (d > radius)
		discard;

	color = vec4(1.0, 0.0, 0.0, 1.0);
}