#pragma once

#include "types.h"
#include "viewport.h"

namespace poly {

/**
 * @brief      This class draws few squares.
 */
class ScreenDrawer
{
public:
	ScreenDrawer();
	~ScreenDrawer();

	bool Create();
	void Destroy();

	void Render();

	void set_texture(uint32_t texture) { texture_ = texture; }

protected:

	struct Vertex
	{
		Point position;
	};

	bool CreateData();
	void FreeArrays();
	void MakeRenderable();

	void ActivateShader();
	void DeactivateShader();

private:
	uint32_t program_;
	uint32_t vertex_array_object_;
	uint32_t vertex_buffer_object_;
	uint32_t num_vertices_;
	uint8_t* vertices_array_;
	uint32_t texture_;
};

} // namespace poly