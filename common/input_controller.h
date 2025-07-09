#pragma once

// Forward
struct GLFWwindow;

namespace poly {

class InputController {
public:
	InputController() = default;
	virtual ~InputController() = default;

	virtual void ProcessInput(GLFWwindow* window);
	virtual void ProcessMouse(GLFWwindow* window, int button, int action, int mods);
	virtual void ProcessCursor(GLFWwindow* window, double xpos, double ypos);
};

} // namespace poly