#include "app.h"

#include <glad/glad.h>

namespace poly {

Application::Application()
: BaseApplication()
, points_()
, drawer_(&viewport_)
{
}
bool Application::Load()
{
	points_ = {
		{-0.5f,  0.0f, 0.0f},
		{ 0.0f,  0.5f, 0.0f},
		{ 0.0f, -0.5f, 0.0f},
		{ 0.5f,  0.0f, 0.0f}
	};

	if (!drawer_.Create(points_)) return false;

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