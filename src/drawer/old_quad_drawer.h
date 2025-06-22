#pragma once

#include "../types.h"
#include "../viewport.h"

namespace poly {

/**
 * @brief      This class describes a quad drawer without geometry shader.
 */
class OldQuadDrawer
{
public:
	OldQuadDrawer(const Viewport* viewport);
	~OldQuadDrawer();

	bool Create(const PointArray& points);
	void Destroy();

	void Render();

protected:

	struct Vertex
	{
		Point position;
		Point texcoord;
	};

	bool CreateData(const PointArray& points);
	void FreeArrays();
	void MakeRenderable();

	void ActivateShader();
	void DeactivateShader();

private:
	const Viewport* viewport_;
	uint32_t program_;
	uint32_t vertex_array_object_;
	uint32_t vertex_buffer_object_;
	uint32_t index_buffer_object_;
	uint32_t num_vertices_;
	uint32_t num_indices_;
	uint32_t index_size_;
	uint8_t* vertices_array_;
	uint8_t* indices_array_;
	const float pixel_width_;
};

} // namespace poly