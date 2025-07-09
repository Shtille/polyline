#include "orbit_controller.h"

#include <GLFW/glfw3.h>

namespace poly {

OrbitController::OrbitController(Camera* camera)
: InputController()
, camera_(camera)
, cursor_x_(0.0)
, cursor_y_(0.0)
, left_mouse_button_pressed_(false)
{
	//
}
void OrbitController::ProcessInput(GLFWwindow* window)
{
	InputController::ProcessInput(window);
}
void OrbitController::ProcessMouse(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			left_mouse_button_pressed_ = true;
			OnDragBegin(window);
		}
		else if (action == GLFW_RELEASE)
		{
			left_mouse_button_pressed_ = false;
			OnDragEnd(window);
		}
	}
}
void OrbitController::ProcessCursor(GLFWwindow* window, double xpos, double ypos)
{
	if (left_mouse_button_pressed_)
	{
		OnDragMove(window);
	}
}
void OrbitController::OnDragBegin(GLFWwindow* window)
{
	glfwGetCursorPos(window, &cursor_x_, &cursor_y_);
}
void OrbitController::OnDragEnd(GLFWwindow* window)
{
	RotateCamera(window);
}
void OrbitController::OnDragMove(GLFWwindow* window)
{
	RotateCamera(window);
}
void OrbitController::RotateCamera(GLFWwindow* window)
{
	constexpr float kSpeedX = 0.02f;
	constexpr float kSpeedY = 0.02f;

	double cursor_x, cursor_y;
	glfwGetCursorPos(window, &cursor_x, &cursor_y);
	float delta_x = static_cast<float>(cursor_x - cursor_x_);
	float delta_y = static_cast<float>(cursor_y - cursor_y_);
	cursor_x_ = cursor_x;
	cursor_y_ = cursor_y;
	camera_->RotateY(delta_x * kSpeedX);
	camera_->RotateX(delta_y * kSpeedY);
}

} // namespace poly