#include "dash_polyline_drawer.h"

#include "util.h"

#include <vector>
#include <cmath>

namespace poly {

DashPolylineDrawer::DashPolylineDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, texture_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, texture_width_(0)
, vertices_array_(nullptr)
, texture_data_(nullptr)
, pixel_width_(20.0f)
, miter_limit_(3.0f)
, cap_style_(CapStyle::kFlat)
, join_style_(JoinStyle::kBevel)
{

}
DashPolylineDrawer::~DashPolylineDrawer()
{
	Destroy();
}
bool DashPolylineDrawer::Create(const PointArray& points)
{
	if (!LoadShaders(
		"data/shaders/dash_polyline/polyline.vs", 
		"data/shaders/dash_polyline/polyline.fs", 
		"data/shaders/dash_polyline/polyline.gs", 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void DashPolylineDrawer::Destroy()
{
	FreeArrays();
	if (program_ != 0)
	{
		glDeleteProgram(program_);
		program_ = 0;
	}
	if (texture_ != 0)
	{
		glDeleteTextures(1, &texture_);
		texture_ = 0;
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
void DashPolylineDrawer::Render()
{
	ActivateShader();
	ActivateTexture();

	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_ - 3);
	glBindVertexArray(0);

	DeactivateTexture();
	DeactivateShader();
}
bool DashPolylineDrawer::CreateData(const PointArray& points)
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

	// Texture data
	// ------------
	const std::vector<uint32_t> pattern = {2,1,1,1};
	texture_width_ = 0;
	for (uint32_t value : pattern)
		texture_width_ += value;

	n = 0;
	texture_data_ = new uint8_t[texture_width_];
	for (uint32_t i = 0; i < pattern.size(); ++i)
	{
		uint8_t value = (i % 2) == 0 ? 255 : 0;
		uint8_t count = pattern[i];
		while (count--)
			texture_data_[n++] = value;
	}

	return true;
}
void DashPolylineDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
	if (texture_data_ != nullptr)
	{
		delete[] texture_data_;
		texture_data_ = nullptr;
	}
}
void DashPolylineDrawer::MakeRenderable()
{
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

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

	// Create texture
	// --------------
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_1D, texture_);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_R8, (GLsizei)texture_width_, 0, GL_RED, GL_UNSIGNED_BYTE, texture_data_);

	// Finally
	FreeArrays();
}
void DashPolylineDrawer::ActivateShader()
{
	int location;

	const float dash_unit = pixel_width_;
	const float dash_period = dash_unit * static_cast<float>(texture_width_);

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
	location = glGetUniformLocation(program_, "u_dash_period");
	glUniform1f(location, dash_period);
	location = glGetUniformLocation(program_, "u_texture");
	glUniform1i(location, 0);
}
void DashPolylineDrawer::DeactivateShader()
{
	glUseProgram(0);
}
void DashPolylineDrawer::ActivateTexture()
{
	glBindTexture(GL_TEXTURE_1D, texture_);
}
void DashPolylineDrawer::DeactivateTexture()
{
	glBindTexture(GL_TEXTURE_1D, 0);
}
void DashPolylineDrawer::SetCapStyle(CapStyle cap_style)
{
	cap_style_ = cap_style;
}
void DashPolylineDrawer::SetJoinStyle(JoinStyle join_style)
{
	join_style_ = join_style;
}
CapStyle DashPolylineDrawer::cap_style() const
{
	return cap_style_;
}
JoinStyle DashPolylineDrawer::join_style() const
{
	return join_style_;
}

} // namespace poly