#pragma once

#include "../types.h"
#include "../viewport.h"

namespace poly {

/**
 * @brief      This class describes a quad drawer with geometry shader.
 */
class SolidPolylineDrawer
{
public:
	SolidPolylineDrawer(const Viewport* viewport);
	~SolidPolylineDrawer();

	bool Create(const PointArray& points);
	void Destroy();

	void Render();

	void SetCapStyle(CapStyle cap_style);
	void SetJoinStyle(JoinStyle join_style);

	CapStyle cap_style() const;
	JoinStyle join_style() const;

protected:

	struct Vertex
	{
		Point position;
		float point_type;
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
	float miter_limit_;
	CapStyle cap_style_;
	JoinStyle join_style_;
};

} // namespace poly