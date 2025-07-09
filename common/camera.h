#pragma once

#include "viewport.h"

#include <glm/mat4x4.hpp>

namespace poly {

class Camera {
public:
	explicit Camera(const Viewport* viewport, float distance);

	void Update();

	void RotateX(float value);
	void RotateY(float value);

	const glm::mat4& projection_view_matrix() const;

private:
	const Viewport* viewport_;
	glm::mat4 projection_view_matrix_;
	float angle_x_;
	float angle_y_;
	float distance_;
	bool matrix_dirty_;
};

} // namespace poly