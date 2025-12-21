#include "object_drawer.h"

#include "util.h"

#include "../microstl.h"

#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <set>
#include <cmath>
#include <cstdio>

namespace poly {

ObjectDrawer::ObjectDrawer(const Viewport* viewport, const Camera* camera)
: viewport_(viewport)
, camera_(camera)
, program_(0)
, vertex_array_object_(0)
, vertex_buffer_object_(0)
, index_buffer_object_(0)
, num_vertices_(0)
, num_indices_(0)
, index_size_(0)
, vertices_array_(nullptr)
, indices_array_(nullptr)
, color_({0.0f, 0.0f, 0.0f, 1.0f})
, bbox_()
{

}
ObjectDrawer::~ObjectDrawer()
{
	Destroy();
}
bool ObjectDrawer::Create(const char* filename)
{
	if (!LoadShaders(
		"data/shaders/object/object.vs", 
		"data/shaders/object/object.fs", 
		nullptr, 
		program_))
		return false;

	if (!CreateData(filename))
		return false;
	MakeRenderable();

	return true;
}
void ObjectDrawer::Destroy()
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
void ObjectDrawer::Render()
{
	ActivateShader();

	glBindVertexArray(vertex_array_object_);

	glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);

	DeactivateShader();
}
bool ObjectDrawer::CreateData(const char* filename)
{
	// Use included handler that creates a simple mesh data structure
	microstl::MeshReaderHandler meshHandler;

	// Start parsing the file and let the data go into the mesh handler
	microstl::Result result = microstl::Reader::readStlFile(filename, meshHandler);

	// Check if the parsing was successful or if there was an error
	if (result != microstl::Result::Success)
	{
		printf("Failed loading file '%s'\n", filename);
		return false;
	}

	// Now the extracted mesh data can be accessed
	const microstl::Mesh& mesh = meshHandler.mesh;

	if (mesh.facets.size() == 0)
	{
		printf("No vertices in file '%s'\n", filename);
		return false;
	}

	num_vertices_ = static_cast<uint32_t>(mesh.facets.size() * 3);
	vertices_array_ = new uint8_t[num_vertices_ * sizeof(Vertex)];
	Vertex* vertices_data = reinterpret_cast<Vertex*>(vertices_array_);

	num_indices_ = num_vertices_;
	index_size_ = sizeof(uint32_t);
	indices_array_ = new uint8_t[num_indices_ * index_size_];
	uint32_t* indices_data = reinterpret_cast<uint32_t*>(indices_array_);

	// Initial values
	bbox_.min.x = mesh.facets[0].v1.x;
	bbox_.min.y = mesh.facets[0].v1.y;
	bbox_.min.z = mesh.facets[0].v1.z;
	bbox_.max = bbox_.min;

	// Position
	uint32_t n = 0;
	for (const microstl::Facet& facet : mesh.facets)
	{
		{
			glm::vec3& pos = vertices_data[n].position;
			pos.x = facet.v1.x;
			pos.y = facet.v1.y;
			pos.z = facet.v1.z;
			MakeMinimum(pos, bbox_.min);
			MakeMaximum(pos, bbox_.max);
			++n;
		}
		{
			glm::vec3& pos = vertices_data[n].position;
			pos.x = facet.v2.x;
			pos.y = facet.v2.y;
			pos.z = facet.v2.z;
			MakeMinimum(pos, bbox_.min);
			MakeMaximum(pos, bbox_.max);
			++n;
		}
		{
			glm::vec3& pos = vertices_data[n].position;
			pos.x = facet.v3.x;
			pos.y = facet.v3.y;
			pos.z = facet.v3.z;
			MakeMinimum(pos, bbox_.min);
			MakeMaximum(pos, bbox_.max);
			++n;
		}
	}

	// Indices
	for (uint32_t i = 0; i < num_indices_; ++i)
	{
		indices_data[i] = i;
	}

	return true;
}
void ObjectDrawer::FreeArrays()
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
void ObjectDrawer::MakeRenderable()
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

	// Finally
	FreeArrays();
}
void ObjectDrawer::ActivateShader()
{
	int location;

	glUseProgram(program_);

	location = glGetUniformLocation(program_, "u_mvp");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera_->projection_view_matrix()));

	location = glGetUniformLocation(program_, "u_color");
	glUniform4fv(location, 1, &color_[0]);
}
void ObjectDrawer::DeactivateShader()
{
	glUseProgram(0);
}

} // namespace poly