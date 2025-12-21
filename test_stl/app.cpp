#include "app.h"

#include <glad/glad.h>

static void SetInitialStates()
{
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

namespace poly {

Application::Application()
: BaseApplication()
, camera_(&viewport_, 5.0f)
, controller_(&camera_)
, drawer_(&viewport_, &camera_)
, background_color_({1.0f, 1.0f, 1.0f, 1.0f})
{
}
bool Application::Load()
{
	if (!drawer_.Create("data/models/cube_ascii.stl")) return false;

	// Center camera
	const auto& bbox = drawer_.bounding_box();
	camera_.ObserveObject(bbox.min, bbox.max);

	// Set initial OpenGL states
	::SetInitialStates();

	return true;
}
void Application::Unload()
{
	drawer_.Destroy();
}
void Application::Update()
{
	camera_.Update();
}
void Application::Render()
{
	glClearColor(
		background_color_[0], 
		background_color_[1],
		background_color_[2],
		background_color_[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawer_.Render();
}
InputController* Application::controller()
{
	return &controller_;
}

} // namespace poly