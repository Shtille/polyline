#include "old_quad_drawer.h"

#include "util.h"

namespace poly {

OldQuadDrawer::OldQuadDrawer(const Viewport* viewport)
: viewport_(viewport)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, index_buffer_object_(0)
, num_vertices_(0)
, num_indices_(0)
, index_size_(0)
, vertices_array_(nullptr)
, indices_array_(nullptr)
, pixel_width_(20.0f)
{

}
OldQuadDrawer::~OldQuadDrawer()
{
	Destroy();
}
bool OldQuadDrawer::Create(const PointArray& points)
{
	if (!LoadShaders(
		"data/shaders/quad/old_quad.vs", 
		"data/shaders/quad/old_quad.fs", 
		nullptr, 
		program_))
		return false;

	if (!CreateData(points))
		return false;
	MakeRenderable();

	return true;
}
void OldQuadDrawer::Destroy()
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
	if (index_buffer_object_ != 0)
	{
		glDeleteBuffers(1, &index_buffer_object_);
		index_buffer_object_ = 0;
	}
	if (vertex_array_object_ != 0)
	{
		glDeleteVertexArrays(1, &vertex_array_object_);
		vertex_array_object_ = 0;
	}
}
void OldQuadDrawer::Render()
{
	ActivateShader();

	glBindVertexArray(vertex_array_object_);
	glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	DeactivateShader();
}
bool OldQuadDrawer::CreateData(const PointArray& points)
{
	uint32_t num_points = static_cast<uint32_t>(points.size());
	if (num_points < 2) return false;
	uint32_t num_segments = num_points - 1;
	uint32_t total_segments = num_segments + 2;

	num_vertices_ = 4 * total_segments;
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices = reinterpret_cast<Vertex*>(vertices_array_);

	num_indices_ = 6 * num_segments;
	index_size_ = sizeof(uint32_t);
	indices_array_ = new uint8_t[num_indices_ * index_size_];
	uint32_t* indices = reinterpret_cast<uint32_t*>(indices_array_);

	// Positions
	uint32_t n = 0;
	Point first_point, last_point;
	first_point[0] = points[0][0] + (points[0][0] - points[1][0]);
	first_point[1] = points[0][1] + (points[0][1] - points[1][1]);
	last_point[0] = points[num_points - 1][0] + (points[num_points - 1][0] - points[num_points - 2][0]);
	last_point[1] = points[num_points - 1][1] + (points[num_points - 1][1] - points[num_points - 2][1]);
	vertices[n++].position = first_point;
	vertices[n++].position = first_point;
	vertices[n++].position = points[0];
	vertices[n++].position = points[0];
	for (uint32_t i = 0; i < num_segments; ++i)
	{
		vertices[n++].position = points[i];
		vertices[n++].position = points[i];
		vertices[n++].position = points[i+1];
		vertices[n++].position = points[i+1];
	}
	vertices[n++].position = points[num_points - 1];
	vertices[n++].position = points[num_points - 1];
	vertices[n++].position = last_point;
	vertices[n++].position = last_point;

	// Texcoords
	n = 0;
	for (uint32_t i = 0; i < total_segments; ++i)
	{
		vertices[n++].texcoord = { 0.0f,  1.0f};
		vertices[n++].texcoord = { 0.0f, -1.0f};
		vertices[n++].texcoord = { 0.0f,  1.0f};
		vertices[n++].texcoord = { 0.0f, -1.0f};
	}

	// Indices
	n = 0;
	for (uint32_t i = 0; i < num_segments; ++i)
	{
		// 0-1-2
		indices[n++] = 4*i+0;
		indices[n++] = 4*i+1;
		indices[n++] = 4*i+2;
		// 2-1-3
		indices[n++] = 4*i+2;
		indices[n++] = 4*i+1;
		indices[n++] = 4*i+3;
	}

	return true;
}
void OldQuadDrawer::FreeArrays()
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
}
void OldQuadDrawer::MakeRenderable()
{
	glGenVertexArrays(1, &vertex_array_object_);
	glBindVertexArray(vertex_array_object_);

	glGenBuffers(1, &vertex_buffer_object_);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
	glBufferData(GL_ARRAY_BUFFER, num_vertices_ * sizeof(Vertex), vertices_array_, GL_STATIC_DRAW);

	glGenBuffers(1, &index_buffer_object_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices_ * index_size_, indices_array_, GL_STATIC_DRAW);

	FreeArrays();

	static_assert(sizeof(Point) == 2*sizeof(float), "Point size mismatch");

	// Attributes layout
	const GLsizei stride = sizeof(Vertex);
	const uint8_t* base = nullptr;
	const uint8_t* curr_offset = base + 4*stride; // offset to start of actual current data
	const uint8_t* prev_offset = curr_offset - 2*stride;
	const uint8_t* next_offset = curr_offset + 2*stride;
	const uint8_t* texcoord_offset = curr_offset + sizeof(Point);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, prev_offset); // vec2 a_prev
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, curr_offset); // vec2 a_curr
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, next_offset); // vec2 a_next
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, texcoord_offset); // vec2 a_texcoord;
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}
void OldQuadDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);
	location = glGetUniformLocation(program_, "u_viewport");
	glUniform4f(location, 0.0f, 0.0f, (float)viewport_->width, (float)viewport_->height);
	location = glGetUniformLocation(program_, "u_width");
	glUniform1f(location, pixel_width_);
}
void OldQuadDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly