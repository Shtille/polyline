#include "camera.h"

#include "util.h"

#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include <cmath>
#include <cstdio>

namespace poly {

Camera::Camera(const Viewport* viewport, float distance)
: viewport_(viewport)
, projection_view_matrix_()
, center_(0.0f)
, fovy_(glm::pi<float>() * 0.25f)
, angle_x_(0.0f)
, angle_y_(0.0f)
, distance_(distance)
, observe_multiplier_(2.0f)
, matrix_dirty_(true)
{

}
void Camera::Update()
{
	if (!matrix_dirty_) return;
	matrix_dirty_ = false;

	glm::mat4 projection = glm::perspective(fovy_, viewport_->aspect_ratio, 0.1f, 100.f);

	// glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -distance_));
	// view = glm::rotate(view, angle_x_, glm::vec3(1.0f, 0.0f, 0.0f));
	// view = glm::rotate(view, angle_y_, glm::vec3(0.0f, 1.0f, 0.0f));
	// view = glm::translate(view, center_);

	// Calculate the camera's position relative to the target point
    // Assume initial camera position at the origin, then rotate and translate
    glm::vec3 offset(0.0f, 0.0f, -distance_); // start at a point along -Z

    // Create rotation matrices
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angle_x_, glm::vec3(1,0,0));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), -angle_y_, glm::vec3(0,1,0));

    // Combine rotations
    glm::mat4 rotation = rotationY * rotationX;

    // Rotate offset
    glm::vec4 rotatedOffset = rotation * glm::vec4(offset, 1.0f);

    // Camera position is target + rotated offset
    glm::vec3 cameraPos = center_ + glm::vec3(rotatedOffset);

    // Create the view matrix using lookAt
    glm::mat4 view = glm::lookAt(
        cameraPos,   // Camera position
        center_,      // Target point the camera looks at
        glm::vec3(0.0f, 1.0f, 0.0f) // Up vector
    );

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
void Camera::ObserveObject(const glm::vec3& min_pos, const glm::vec3& max_pos)
{
	glm::vec3 center = 0.5f * (min_pos + max_pos);
	float size = MaximumComponent(0.5f * (max_pos - min_pos));

	// Statistics information
	printf("Minimum (%.2f, %.2f, %.2f)\n", min_pos.x, min_pos.y, min_pos.z);
	printf("Maximum (%.2f, %.2f, %.2f)\n", max_pos.x, max_pos.y, max_pos.z);
	printf("Center (%.2f, %.2f, %.2f)\n", center.x, center.y, center.z);
	printf("Size (%.2f)\n", size);

	center_ = center;
	distance_ = size * observe_multiplier_ / tan(fovy_ * 0.5f);
	matrix_dirty_ = true;
}
const glm::mat4& Camera::projection_view_matrix() const
{
	return projection_view_matrix_;
}

} // namespace poly