#pragma once

#include <array>

#include "types.h"
#include "viewport.h"
#include "camera.h"

namespace poly {

/**
 * @brief      This class describes a wireframe drawer.
 */
class WireframeDrawer
{
public:
	WireframeDrawer(const Viewport* viewport, const Camera* camera);
	~WireframeDrawer();

	bool Create(const Point3DArray& vertices, const IndicesArray& indices);
	void Destroy();

	void Render();

protected:

	struct Vertex
	{
		Point3D position;
	};

	bool CreateData(const Point3DArray& vertices, const IndicesArray& indices);
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
	float pixel_width_;
	std::array<float, 4> color_;
};

} // namespace poly