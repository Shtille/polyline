#pragma once

#include "types.h"
#include "viewport.h"

namespace poly {

/**
 * @brief      This class draws few squares.
 */
class SquareDrawer
{
public:
	SquareDrawer(const Viewport* viewport);
	~SquareDrawer();

	struct Vertex
	{
		Point position;
		float size;
		Color color;
	};
	typedef std::vector<Vertex> VertexArray;

	bool Create(const VertexArray& vertices);
	void Destroy();

	void Render();

protected:

	bool CreateData(const VertexArray& vertices);
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
};

} // namespace poly