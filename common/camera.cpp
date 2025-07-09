#include "camera.h"

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

namespace poly {

Camera::Camera(const Viewport* viewport, float distance)
: viewport_(viewport)
, projection_view_matrix_()
, angle_x_(0.0f)
, angle_y_(0.0f)
, distance_(distance)
, matrix_dirty_(true)
{

}
void Camera::Update()
{
	if (!matrix_dirty_) return;
	matrix_dirty_ = false;

	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, viewport_->aspect_ratio, 0.1f, 100.f);

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -distance_));
	view = glm::rotate(view, angle_x_, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, angle_y_, glm::vec3(0.0f, 1.0f, 0.0f));

	projection_view_matrix_ = projection * view;
}
void Camera::RotateX(float value)
{
	angle_x_ += value;
	matrix_dirty_ = true;
}
void Camera::RotateY(float value)
{
	angle_y_ += value;
	matrix_dirty_ = true;
}
const glm::mat4& Camera::projection_view_matrix() const
{
	return projection_view_matrix_;
}

} // namespace poly