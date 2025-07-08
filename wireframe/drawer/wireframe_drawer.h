#pragma once

#include "types.h"
#include "viewport.h"

namespace poly {

/**
 * @brief      This class describes a wireframe drawer.
 */
class WireframeDrawer
{
public:
	WireframeDrawer(const Viewport* viewport);
	~WireframeDrawer();

	bool Create(const Point3DArray& points);
	void Destroy();

	void Render();

protected:

	struct Vertex
	{
		Point3D position;
	};

	bool CreateData(const Point3DArray& points);
	void FreeArrays();
	void MakeRenderable();

	void ActivateShader();
	void DeactivateShader();

private:
	const Viewport* viewport_;
	uint32_t program_;
	uint32_t vertex_array_object_;
	uint32_t vertex_buffer_object_;
	uint32_t num_vertices_;
	uint8_t* vertices_array_;
	float pixel_width_;
};

} // namespace poly