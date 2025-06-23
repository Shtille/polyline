#include "types.h"
#include "viewport.h"
#include "drawer/quad_drawer.h"
#include "drawer/old_quad_drawer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

namespace {
	poly::Viewport viewport;
	poly::QuadDrawer* new_drawer = nullptr;
	poly::OldQuadDrawer* old_drawer = nullptr;
	poly::PointArray points;

	void SetViewport(int width, int height)
	{
		viewport.width = width;
		viewport.height = height;
		viewport.aspect_ratio = (float)viewport.width / (float)viewport.height;
	}
	bool Load()
	{
		points = {
			{-0.5f,  0.0f},
			{ 0.0f,  0.5f},
			{ 0.0f, -0.5f},
			{ 0.5f,  0.0f}
		};

		new_drawer = new poly::QuadDrawer(&viewport);
		old_drawer = new poly::OldQuadDrawer(&viewport);

		if (!new_drawer->Create(points)) return false;
		if (!old_drawer->Create(points)) return false;

		return true;
	}
	void Unload()
	{
		new_drawer->Destroy();
		old_drawer->Destroy();
		delete new_drawer;
		delete old_drawer;
	}
	void Render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		old_drawer->Render();
	}
}

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

	// Set viewport
	// ---------------------------
	SetViewport(800, 600);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(viewport.width, viewport.height, "Performance test", NULL, NULL);
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
	if (!Load())
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
		Render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Unload application data
	// -----------------------
	Unload();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SetViewport(width, height);
}