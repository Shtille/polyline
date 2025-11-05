#include "screen_drawer.h"

#include "util.h"

namespace poly {

ScreenDrawer::ScreenDrawer()
: program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, num_vertices_(0)
, vertices_array_(nullptr)
, texture_(0)
{

}
ScreenDrawer::~ScreenDrawer()
{
	Destroy();
}
bool ScreenDrawer::Create()
{
	if (!LoadShaders(
		"data/shaders/screen/screen.vs", 
		"data/shaders/screen/screen.fs", 
		nullptr, //"data/shaders/screen/screen.gs", 
		program_))
		return false;

	if (!CreateData())
		return false;
	MakeRenderable();

	return true;
}
void ScreenDrawer::Destroy()
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
void ScreenDrawer::Render()
{
	ActivateShader();

	glBindTexture(GL_TEXTURE_2D, texture_);

	glDepthMask(GL_FALSE);
	glBindVertexArray(vertex_array_object_);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices_);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, 0);

	DeactivateShader();
}
bool ScreenDrawer::CreateData()
{
	std::vector<Vertex> vertices = {
		{-1.0f,  1.0f},
		{-1.0f, -1.0f},
		{ 1.0f,  1.0f},
		{ 1.0f, -1.0f},
	};

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
void ScreenDrawer::FreeArrays()
{
	if (vertices_array_ != nullptr)
	{
		delete[] vertices_array_;
		vertices_array_ = nullptr;
	}
}
void ScreenDrawer::MakeRenderable()
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, position_offset); // vec2 a_position
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
void ScreenDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);

	location = glGetUniformLocation(program_, "u_texture");
	glUniform1i(location, 0);
}
void ScreenDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly