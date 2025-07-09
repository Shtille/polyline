#include "input_controller.h"

#include <GLFW/glfw3.h>

namespace poly {

void InputController::ProcessInput(GLFWwindow* window)
{
	// Terminate on Escape key
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void InputController::ProcessMouse(GLFWwindow* window, int button, int action, int mods)
{
	//
}
void InputController::ProcessCursor(GLFWwindow* window, double xpos, double ypos)
{
	//
}

} // namespace poly