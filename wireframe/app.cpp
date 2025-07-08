#include "app.h"

#include <glad/glad.h>

namespace poly {

Application::Application()
: BaseApplication()
, drawer_(&viewport_)
{
}
bool Application::Load()
{
	Point3DArray vertices = {
		{-0.5f,  0.0f, 0.0f},
		{ 0.0f,  0.5f, 0.0f},
		{ 0.0f, -0.5f, 0.0f},
		{ 0.5f,  0.0f, 0.0f},
	};
	IndicesArray indices = {
		0, 1,
		1, 2,
		2, 3,
	};

	if (!drawer_.Create(vertices, indices)) return false;

	return true;
}
void Application::Unload()
{
	drawer_.Destroy();
}
void Application::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawer_.Render();
}

} // namespace poly