#pragma once

#include <array>

#include <glm/vec3.hpp>

#include "types.h"
#include "viewport.h"
#include "camera.h"

namespace poly {

/**
 * @brief      This class describes a wireframe drawer.
 */
class ObjectDrawer
{
public:
	ObjectDrawer(const Viewport* viewport, const Camera* camera);
	~ObjectDrawer();

	bool Create(const char* filename);
	void Destroy();

	void Render();

	struct BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	const BoundingBox& bounding_box() const { return bbox_; }

protected:

	struct Vertex
	{
		glm::vec3 position;
	};

	bool CreateData(const char* filename);
	void FreeArrays();
	void MakeRenderable();

	void ActivateShader();
	void DeactivateShader();

private:
	const Viewport* viewport_;
	const Camera* camera_;
	uint32_t program_;
	uint32_t vertex_array_object_;
	uint32_t vertex_buffer_object_;
	uint32_t index_buffer_object_;
	uint32_t num_vertices_;
	uint32_t num_indices_;
	uint32_t index_size_;
	uint8_t* vertices_array_;
	uint8_t* indices_array_;
	std::array<float, 4> color_;
	BoundingBox bbox_;
};

} // namespace poly