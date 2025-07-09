#include "app.h"

#include <glad/glad.h>

namespace poly {

Application::Application()
: BaseApplication()
, camera_(&viewport_, 5.0f)
, controller_(&camera_)
, drawer_(&viewport_, &camera_)
{
}
bool Application::Load()
{
	const float cube_size = 1.0f;
	float w = 0.5f * cube_size; // half cube side
	Point3DArray vertices = {
		// front
		{-w, -w,  w},
		{ w, -w,  w},
		{-w,  w,  w},
		{ w,  w,  w},
		// back
		{-w, -w, -w},
		{ w, -w, -w},
		{-w,  w, -w},
		{ w,  w, -w},
	};
	IndicesArray indices = {
		// front
		0, 1,
		1, 3,
		0, 2,
		2, 3,
		// back
		4, 5,
		5, 7,
		4, 6,
		6, 7,
		// sides
		0, 4,
		1, 5,
		2, 6,
		3, 7,
	};

	if (!drawer_.Create(vertices, indices)) return false;

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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawer_.Render();
}
InputController* Application::controller()
{
	return &controller_;
}

} // namespace poly