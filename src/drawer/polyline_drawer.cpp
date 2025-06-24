#include "polyline_drawer.h"

#include "../util.h"

#include <cmath>

namespace poly {

PolylineDrawer::PolylineDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, vertices_array_(nullptr)
, pixel_width_(20.0f)
, miter_limit_(3.0f)
, cap_style_(CapStyle::kRound)
, join_style_(JoinStyle::kRound)
{

}
PolylineDrawer::~PolylineDrawer()
{
	Destroy();
}
bool PolylineDrawer::Create(const PointArray& points)
{
	if (!LoadShaders(
		"data/shaders/polyline/polyline.vs", 
		"data/shaders/polyline/polyline.fs", 
		"data/shaders/polyline/polyline.gs", 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void PolylineDrawer::Destroy()
{
	FreeArrays();
	if (program_ != 0)
	{
		glDeleteProgram(program_);
		program_ = 0;
	}
	if (vertex_buffer_object_ != 0)
	{
		glDeleteBuffers(1, &vertex_buffer_object_);
		vertex_buffer_object_ = 0;
	}
	if (vertex_array_object_ != 0)
	{
		glDeleteVertexArrays(1, &vertex_array_object_);
		vertex_array_object_ = 0;
	}
}
void PolylineDrawer::Render()
{
	ActivateShader();

	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_ - 3);
	glBindVertexArray(0);

	DeactivateShader();
}
bool PolylineDrawer::CreateData(const PointArray& points)
{
	uint32_t num_points = static_cast<uint32_t>(points.size());
	if (num_points < 2) return false;

	// We add one point before and one point after to have access to previous and next segments.
	num_vertices_ = num_points + 2;
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices = reinterpret_cast<Vertex*>(vertices_array_);

	// Position
	uint32_t n = 0;
	// First point is extrapolated one from the first segment.
	Point first_point;
	first_point[0] = points[0][0] + (points[0][0] - points[1][0]);
	first_point[1] = points[0][1] + (points[0][1] - points[1][1]);
	vertices[n++].position = first_point;
	for (uint32_t i = 0; i < num_points; ++i)
	{
		const Point& point = points[i];
		vertices[n++].position = {point[0], point[1]};
	}
	// The last point is extrapolated one from the last segment.
	Point last_point;
	last_point[0] = points[num_points-1][0] + (points[num_points-1][0] - points[num_points-2][0]);
	last_point[1] = points[num_points-1][1] + (points[num_points-1][1] - points[num_points-2][1]);
	vertices[n++].position = last_point;

	// Point type
	n = 0;
	vertices[n++].point_type = 0.0f; // value here doesn't matter
	for (uint32_t i = 0; i < num_points; ++i)
	{
		vertices[n++].point_type = (i == 0) ? -1.0f : (i == num_points - 1) ? 1.0f : 0.0f;
	}
	vertices[n++].point_type = 0.0f; // value here doesn't matter

	// Distance
	n = 0;
	vertices[n++].distance = 0.0f;
	vertices[n++].distance = 0.0f; // point 0
	for (uint32_t i = 1; i < num_points; ++i)
	{
		const Point& point1 = points[i-1];
		const Point& point2 = points[i];
		float dx = point2[0] - point1[0];
		float dy = point2[1] - point1[1];
		vertices[n++].distance = sqrt(dx * dx + dy * dy);
	}
	vertices[n++].distance = vertices[n-1].distance;

	return true;
}
void PolylineDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
}
void PolylineDrawer::MakeRenderable()
{
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

	FreeArrays();

	static_assert(sizeof(Point) == 2*sizeof(float), "Point size mismatch");

	// Attributes layout
	const GLsizei stride = sizeof(Vertex);
	const uint8_t* base = nullptr;
	const uint8_t* prev_offset = base;
	const uint8_t* curr_offset = prev_offset + stride;
	const uint8_t* next_offset = curr_offset + stride;
	const uint8_t* point_type_curr_offset = curr_offset + sizeof(Point);
	const uint8_t* point_type_next_offset = point_type_curr_offset + stride;
	const uint8_t* distance_curr_offset = point_type_curr_offset + sizeof(float);
	const uint8_t* distance_next_offset = distance_curr_offset + stride;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, prev_offset); // vec2 a_position_prev
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec2 a_position_curr
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, next_offset); // vec2 a_position_next
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, point_type_curr_offset); // float a_point_type_curr
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, point_type_next_offset); // float a_point_type_next
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, stride, distance_curr_offset); // float a_distance_curr
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, stride, distance_next_offset); // float a_distance_next
	glEnableVertexAttribArray(6);

	glBindVertexArray(0);
}
void PolylineDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);
	location = glGetUniformLocation(program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
	location = glGetUniformLocation(program_, "u_miter_limit");
	glUniform1f(location, miter_limit_);
	location = glGetUniformLocation(program_, "u_cap_style");
	glUniform1i(location, static_cast<int>(cap_style_));
	location = glGetUniformLocation(program_, "u_join_style");
	glUniform1i(location, static_cast<int>(join_style_));
}
void PolylineDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly