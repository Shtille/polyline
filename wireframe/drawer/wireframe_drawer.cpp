#include "wireframe_drawer.h"

#include "util.h"

#include <vector>
#include <cmath>

namespace poly {

WireframeDrawer::WireframeDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, vertices_array_(nullptr)
, pixel_width_(20.0f)
{

}
WireframeDrawer::~WireframeDrawer()
{
	Destroy();
}
bool WireframeDrawer::Create(const Point3DArray& points)
{
	if (!LoadShaders(
		"data/shaders/wireframe/wireframe.vs", 
		"data/shaders/wireframe/wireframe.fs", 
		"data/shaders/wireframe/wireframe.gs", 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void WireframeDrawer::Destroy()
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
void WireframeDrawer::Render()
{
	ActivateShader();

	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_ - 1);
	glBindVertexArray(0);

	DeactivateShader();
}
bool WireframeDrawer::CreateData(const Point3DArray& points)
{
	uint32_t num_points = static_cast<uint32_t>(points.size());
	if (num_points < 2) return false;

	static_assert(sizeof(Vertex) == sizeof(Point3D), "Vertex size mismatch");

	// We add one point before and one point after to have access to previous and next segments.
	num_vertices_ = num_points;
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices = reinterpret_cast<Vertex*>(vertices_array_);

	// Position
	uint32_t n = 0;
	for (uint32_t i = 0; i < num_points; ++i)
	{
		vertices[n++].position = points[i];
	}

	return true;
}
void WireframeDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
}
void WireframeDrawer::MakeRenderable()
{
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

	static_assert(sizeof(Point3D) == 3*sizeof(float), "Point size mismatch");

	// Attributes layout
	const GLsizei stride = sizeof(Vertex);
	const uint8_t* base = nullptr;
	const uint8_t* curr_offset = base;
	const uint8_t* next_offset = curr_offset + stride;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec3 a_position_curr
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, next_offset); // vec3 a_position_next
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Finally
	FreeArrays();
}
void WireframeDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);
	location = glGetUniformLocation(program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
}
void WireframeDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly