#include "square_drawer.h"

#include "util.h"

namespace poly {

SquareDrawer::SquareDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, vertices_array_(nullptr)
{

}
SquareDrawer::~SquareDrawer()
{
	Destroy();
}
bool SquareDrawer::Create(const VertexArray& vertices)
{
	if (!LoadShaders(
		"data/shaders/square/square.vs", 
		"data/shaders/square/square.fs", 
		"data/shaders/square/square.gs", 
		program_))
		return false;

	if (!CreateData(vertices))
		return false;
	MakeRenderable();

	return true;
}
void SquareDrawer::Destroy()
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
void SquareDrawer::Render()
{
	ActivateShader();

	glDepthMask(GL_FALSE);
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_POINTS, 0, num_vertices_);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	DeactivateShader();
}
bool SquareDrawer::CreateData(const VertexArray& vertices)
{
	num_vertices_ = static_cast<uint32_t>(vertices.size());
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices_data = reinterpret_cast<Vertex*>(vertices_array_);
	if (!vertices_array_)
		return false;

	for (uint32_t i = 0; i < num_vertices_; ++i)
	{
		vertices_data[i] = vertices[i];
	}

	return true;
}
void SquareDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
}
void SquareDrawer::MakeRenderable()
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
	const uint8_t* position_offset = base;
	const uint8_t* size_offset = base + sizeof(Point);
	const uint8_t* color_offset = size_offset + sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, position_offset); // vec2 a_position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, size_offset); // float a_size
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, color_offset); // vec4 a_color
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
void SquareDrawer::ActivateShader()
{
	glUseProgram(program_);
}
void SquareDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly