#include "drawer.h"

#include "util.h"

namespace poly {

Drawer::Drawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, vertices_array_(nullptr)
{

}
Drawer::~Drawer()
{
	Destroy();
}
bool Drawer::Create(const PointArray& points)
{
	if (!LoadShaders(
		"data/shaders/point/point.vs", 
		"data/shaders/point/point.fs", 
		"data/shaders/point/point.gs", 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void Drawer::Destroy()
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
void Drawer::Render()
{
	ActivateShader();

	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_);
	glBindVertexArray(0);

	DeactivateShader();
}
bool Drawer::CreateData(const PointArray& points)
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

		vertex.point_x = point[0];
		vertex.point_y = point[1];
	}

	return true;
}
void Drawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
}
void Drawer::MakeRenderable()
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, base); // vec2 a_position
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
void Drawer::ActivateShader()
{
	glUseProgram(program_);
	int location = glGetUniformLocation(program_, "u_aspect_ratio");
	glUniform1f(location, viewport_->aspect_ratio);
}
void Drawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly