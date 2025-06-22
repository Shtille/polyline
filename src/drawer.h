#pragma once

#include "types.h"
#include "viewport.h"

namespace poly {

class Drawer
{
public:
	Drawer(const Viewport* viewport);
	~Drawer();

	bool Create(const PointArray& points);
	void Destroy();

	void Render();

protected:

	struct Vertex
	{
		float point_x;
		float point_y;
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
	uint32_t num_vertices_;
	uint8_t* vertices_array_;
	const float pixel_width_;
};

} // namespace poly