#include "app.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::string get_title();

static const int kCapKey = GLFW_KEY_C;
static const int kJoinKey = GLFW_KEY_J;
static const int kShowKey = GLFW_KEY_SPACE;
static poly::Application* s_app = nullptr;
static bool s_cap_key_pressed = false;
static bool s_join_key_pressed = false;
static bool s_show_key_pressed = false;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create application instance
	// ---------------------------
	poly::Application app;
	s_app = &app;

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(app.width(), app.height(), get_title().c_str(), NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Load application data
	// ---------------------
	if (!app.Load())
	{
		glfwTerminate();
		return 1;
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		app.Render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Unload application data
	// -----------------------
	app.Unload();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

static poly::CapStyle GetNextCapStyle(poly::CapStyle style)
{
	uint32_t curr = static_cast<uint32_t>(style);
	uint32_t next = (curr + 1) % 3;
	return static_cast<poly::CapStyle>(next);
}
static poly::JoinStyle GetNextJoinStyle(poly::JoinStyle style)
{
	uint32_t curr = static_cast<uint32_t>(style);
	uint32_t next = (curr + 1) % 3;
	return static_cast<poly::JoinStyle>(next);
}

std::string get_title()
{
	auto cap_style = s_app->cap_style();
	auto join_style = s_app->join_style();
	std::string title = "Polyline (";
	switch (cap_style)
	{
	case poly::CapStyle::kFlat:
		title += "flat cap, ";
		break;
	case poly::CapStyle::kSquare:
		title += "square cap, ";
		break;
	case poly::CapStyle::kRound:
		title += "round cap, ";
		break;
	}
	switch (join_style)
	{
	case poly::JoinStyle::kBevel:
		title += "bevel join)";
		break;
	case poly::JoinStyle::kMiter:
		title += "miter join)";
		break;
	case poly::JoinStyle::kRound:
		title += "round join)";
		break;
	}
	return title;
}

static void UpdateTitle(GLFWwindow *window)
{
	glfwSetWindowTitle(window, get_title().c_str());
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, kCapKey) == GLFW_PRESS && !s_cap_key_pressed)
	{
		s_cap_key_pressed = true;
		s_app->SetCapStyle(GetNextCapStyle(s_app->cap_style()));
		UpdateTitle(window);
	}
	else if (glfwGetKey(window, kCapKey) == GLFW_RELEASE)
	{
		s_cap_key_pressed = false;
	}
	if (glfwGetKey(window, kJoinKey) == GLFW_PRESS && !s_join_key_pressed)
	{
		s_join_key_pressed = true;
		s_app->SetJoinStyle(GetNextJoinStyle(s_app->join_style()));
		UpdateTitle(window);
	}
	else if (glfwGetKey(window, kJoinKey) == GLFW_RELEASE)
	{
		s_join_key_pressed = false;
	}
	if (glfwGetKey(window, kShowKey) == GLFW_PRESS && !s_show_key_pressed)
	{
		s_show_key_pressed = true;
		s_app->SetShow(!s_app->show());
		UpdateTitle(window);
	}
	else if (glfwGetKey(window, kShowKey) == GLFW_RELEASE)
	{
		s_show_key_pressed = false;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	s_app->SetViewport(width, height);
}