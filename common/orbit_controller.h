#pragma once

#include "input_controller.h"
#include "camera.h"

namespace poly {

class OrbitController : public InputController {
public:
	OrbitController(Camera* camera);

	void ProcessInput(GLFWwindow* window) override;
	void ProcessMouse(GLFWwindow* window, int button, int action, int mods) override;
	void ProcessCursor(GLFWwindow* window, double xpos, double ypos) override;

protected:
	void OnDragBegin(GLFWwindow* window);
	void OnDragEnd(GLFWwindow* window);
	void OnDragMove(GLFWwindow* window);

	void RotateCamera(GLFWwindow* window);

private:
	Camera* camera_;
	double cursor_x_;
	double cursor_y_;
	bool left_mouse_button_pressed_;
};

} // namespace poly