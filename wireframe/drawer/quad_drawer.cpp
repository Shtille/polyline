#include "quad_drawer.h"

#include "util.h"

namespace poly {

QuadDrawer::QuadDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, vertices_array_(nullptr)
, pixel_width_(20.0f)
{

}
QuadDrawer::~QuadDrawer()
{
	Destroy();
}
bool QuadDrawer::Create(const PointArray& points)
{
	if (!LoadShaders(
		"data/shaders/quad/quad.vs", 
		"data/shaders/quad/quad.fs", 
		"data/shaders/quad/quad.gs", 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void QuadDrawer::Destroy()
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
void QuadDrawer::Render()
{
	ActivateShader();

	glDepthMask(GL_FALSE);
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_-1);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	DeactivateShader();
}
bool QuadDrawer::CreateData(const PointArray& points)
{
	uint32_t num_points = static_cast<uint32_t>(points.size());
	if (num_points < 2) return false;

	num_vertices_ = num_points;
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices = reinterpret_cast<Vertex*>(vertices_array_);

	for (uint32_t i = 0; i < num_vertices_; ++i)
	{
		Vertex& vertex = vertices[i];
		const Point& point = points[i];

		vertex.position = {point[0], point[1]};
	}

	return true;
}
void QuadDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
}
void QuadDrawer::MakeRenderable()
{
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

	FreeArrays();

	// Attributes layout
	const GLsizei stride = sizeof(Vertex);
	const uint8_t* base = nullptr;
	const uint8_t* curr_offset = base;
	const uint8_t* next_offset = curr_offset + stride;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec2 a_position_curr
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, next_offset); // vec2 a_position_next
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}
void QuadDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);
	location = glGetUniformLocation(program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
}
void QuadDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly