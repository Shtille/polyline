#include "wireframe_drawer.h"

#include "util.h"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <set>
#include <cmath>

namespace poly {

WireframeDrawer::WireframeDrawer(const Viewport* viewport, const Camera* camera)
: viewport_(viewport)
, camera_(camera)
, program_(0)
, quad_program_(0)
, circle_program_(0)
, vertex_array_object_(0)
, circle_vertex_array_object_(0)
, vertex_buffer_object_(0)
, index_buffer_object_(0)
, circle_index_buffer_object_(0)
, num_vertices_(0)
, num_indices_(0)
, circle_num_indices_(0)
, index_size_(0)
, vertices_array_(nullptr)
, indices_array_(nullptr)
, circle_indices_array_(nullptr)
, pixel_width_(20.0f)
, color_({0.0f, 0.0f, 0.0f, 1.0f})
{

}
WireframeDrawer::~WireframeDrawer()
{
	Destroy();
}
bool WireframeDrawer::Create(const Point3DArray& vertices, const IndicesArray& indices)
{
	if (!LoadShaders(
		"data/shaders/wireframe/wireframe.vs", 
		"data/shaders/wireframe/wireframe.fs", 
		"data/shaders/wireframe/wireframe.gs", 
		program_))
		return false;

	if (!LoadShaders(
		"data/shaders/wireframe/wireframe_quad.vs", 
		"data/shaders/wireframe/wireframe_quad.fs", 
		"data/shaders/wireframe/wireframe_quad.gs", 
		quad_program_))
		return false;

	if (!LoadShaders(
		"data/shaders/wireframe/wireframe_circle.vs", 
		"data/shaders/wireframe/wireframe_circle.fs", 
		"data/shaders/wireframe/wireframe_circle.gs", 
		circle_program_))
		return false;

	if (!CreateData(vertices, indices))
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
	if (quad_program_ != 0)
	{
		glDeleteProgram(quad_program_);
		quad_program_ = 0;
	}
	if (circle_program_ != 0)
	{
		glDeleteProgram(circle_program_);
		circle_program_ = 0;
	}
	if (vertex_buffer_object_ != 0)
	{
		glDeleteBuffers(1, &vertex_buffer_object_);
		vertex_buffer_object_ = 0;
	}
	if (index_buffer_object_ != 0)
	{
		glDeleteBuffers(1, &index_buffer_object_);
		index_buffer_object_ = 0;
	}
	if (circle_index_buffer_object_ != 0)
	{
		glDeleteBuffers(1, &circle_index_buffer_object_);
		circle_index_buffer_object_ = 0;
	}
	if (vertex_array_object_ != 0)
	{
		glDeleteVertexArrays(1, &vertex_array_object_);
		vertex_array_object_ = 0;
	}
	if (circle_vertex_array_object_ != 0)
	{
		glDeleteVertexArrays(1, &circle_vertex_array_object_);
		circle_vertex_array_object_ = 0;
	}
}
void WireframeDrawer::Render()
{
	//RenderClassic();
	RenderByCircles();
}
void WireframeDrawer::RenderClassic()
{
	ActivateShader();

	glBindVertexArray(vertex_array_object_);

	glDepthMask(GL_FALSE);
	glDrawElements(GL_LINES, num_indices_, GL_UNSIGNED_INT, nullptr);
	glDepthMask(GL_TRUE);

	glBindVertexArray(0);

	DeactivateShader();
}
void WireframeDrawer::RenderByCircles()
{
	int location;

	glDepthMask(GL_FALSE);

	// Render quads
	glUseProgram(quad_program_);
	location = glGetUniformLocation(quad_program_, "u_mvp");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera_->projection_view_matrix()));
	location = glGetUniformLocation(quad_program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(quad_program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
	location = glGetUniformLocation(quad_program_, "u_color");
	glUniform4fv(location, 1, &color_[0]);

	glBindVertexArray(vertex_array_object_);
	glDrawElements(GL_LINES, num_indices_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// Render circles
	glUseProgram(circle_program_);
	location = glGetUniformLocation(circle_program_, "u_mvp");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera_->projection_view_matrix()));
	location = glGetUniformLocation(circle_program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(circle_program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
	location = glGetUniformLocation(circle_program_, "u_color");
	glUniform4fv(location, 1, &color_[0]);

	glBindVertexArray(circle_vertex_array_object_);
	glDrawElements(GL_POINTS, circle_num_indices_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glUseProgram(0);

	glDepthMask(GL_TRUE);
}
bool WireframeDrawer::CreateData(const Point3DArray& vertices, const IndicesArray& indices)
{
	if (vertices.size() < 2) return false;

	static_assert(sizeof(Vertex) == sizeof(Point3D), "Vertex size mismatch");

	num_vertices_ = static_cast<uint32_t>(vertices.size());
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices_data = reinterpret_cast<Vertex*>(vertices_array_);

	num_indices_ = static_cast<uint32_t>(indices.size());
	index_size_ = sizeof(uint32_t);
	indices_array_ = new uint8_t[num_indices_ * index_size_];
	uint32_t* indices_data = reinterpret_cast<uint32_t*>(indices_array_);

	// Position
	for (uint32_t i = 0; i < num_vertices_; ++i)
	{
		vertices_data[i].position = vertices[i];
	}

	// Indices
	for (uint32_t i = 0; i < num_indices_; ++i)
	{
		indices_data[i] = indices[i];
	}

	// We need list of unique vertex indices for rendering circles
	std::set<uint32_t> unique_indices;
	for (uint32_t index : indices)
		unique_indices.insert(index);

	circle_num_indices_ = static_cast<uint32_t>(unique_indices.size());
	circle_indices_array_ = new uint8_t[circle_num_indices_ * index_size_];
	uint32_t* circle_indices_data = reinterpret_cast<uint32_t*>(circle_indices_array_);

	// Circle indices
	uint32_t n = 0;
	for (uint32_t index : unique_indices)
	{
		circle_indices_data[n++] = index;
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
	if (indices_array_ != nullptr)
	{
		delete[] indices_array_;
		indices_array_ = nullptr;
	}
	if (circle_indices_array_ != nullptr)
	{
		delete[] circle_indices_array_;
		circle_indices_array_ = nullptr;
	}
}
void WireframeDrawer::MakeRenderable()
{
	// Generate render data for lines
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer_object_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_ * index_size_, indices_array_, GL_STATIC_DRAW);

	static_assert(sizeof(Point3D) == 3*sizeof(float), "Point size mismatch");

	// Attributes layout
	{
		const GLsizei stride = sizeof(Vertex);
		const uint8_t* base = nullptr;
		const uint8_t* curr_offset = base;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec3 a_position
		glEnableVertexAttribArray(0);
	}

	glBindVertexArray(0);

	// Generate render data for circles
	glGenVertexArrays(1, &circle_vertex_array_object_);
	glBindVertexArray(circle_vertex_array_object_);

	// vertex buffer already was created before
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);

	glGenBuffers(1, &circle_index_buffer_object_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circle_index_buffer_object_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, circle_num_indices_ * index_size_, circle_indices_array_, GL_STATIC_DRAW);

	// Attributes layout
	{
		const GLsizei stride = sizeof(Vertex);
		const uint8_t* base = nullptr;
		const uint8_t* curr_offset = base;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec3 a_position
		glEnableVertexAttribArray(0);
	}

	glBindVertexArray(0);

	// Finally
	FreeArrays();
}
void WireframeDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);

	location = glGetUniformLocation(program_, "u_mvp");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera_->projection_view_matrix()));

	location = glGetUniformLocation(program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(program_, "u_pixel_width");
	glUniform1f(location, pixel_width_);
	location = glGetUniformLocation(program_, "u_color");
	glUniform4fv(location, 1, &color_[0]);
}
void WireframeDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly