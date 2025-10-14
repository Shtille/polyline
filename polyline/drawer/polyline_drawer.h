#pragma once

#include "types.h"
#include "viewport.h"

namespace poly {

/**
 * @brief      This class describes a quad drawer with geometry shader.
 */
class PolylineDrawer
{
public:
	PolylineDrawer(const Viewport* viewport);
	~PolylineDrawer();

	bool Create(const PointArray& points);
	void Destroy();

	void Render();

protected:

	struct Vertex
	{
		Point position;
	};

	bool CreateData(const PointArray& points);
	void FreeArrays();
	void MakeRenderable();

	void ActivateShader();
	void DeactivateShader();

	void ActivateTexture();
	void DeactivateTexture();

private:
	const Viewport* viewport_;
	uint32_t program_;
	uint32_t texture_;
	uint32_t vertex_array_object_;
	uint32_t vertex_buffer_object_;
	uint32_t num_vertices_;
	uint32_t texture_width_;
	uint8_t* vertices_array_;
	uint8_t* texture_data_;
	const float pixel_width_;
};

} // namespace poly