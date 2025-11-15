#include "app.h"

#include <glad/glad.h>

#define TEST_ANTIALIASING

static void SetInitialStates()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

namespace poly {

Application::Application()
: BaseApplication()
, camera_(&viewport_, 5.0f)
, controller_(&camera_)
, drawer_(&viewport_, &camera_)
, quad_drawer_(&viewport_)
, background_color_({1.0f, 1.0f, 1.0f, 1.0f})
{
}
bool Application::Load()
{
#ifndef TEST_ANTIALIASING
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
#else
	constexpr float d = 0.1f;
	Point3DArray vertices = {
		{1.0f, 0.0f, 0.0f},
		{0.0f,  0.0f*d, 0.0f},
		{0.0f,  5.0f*d, 0.0f},
		{0.0f,  4.0f*d, 0.0f},
		{0.0f,  3.0f*d, 0.0f},
		{0.0f,  2.0f*d, 0.0f},
		{0.0f,  1.0f*d, 0.0f},
		{0.0f, -1.0f*d, 0.0f},
		{0.0f, -2.0f*d, 0.0f},
		{0.0f, -3.0f*d, 0.0f},
		{0.0f, -4.0f*d, 0.0f},
		{0.0f, -5.0f*d, 0.0f},
	};
	IndicesArray indices = {
		0, 1,
		// 0, 2,
		// 0, 3,
		// 0, 4,
		// 0, 5,
		// 0, 6,
		// 0, 7,
		// 0, 8,
		// 0, 9,
		// 0, 10,
		// 0, 11,
	};
#endif

	const float line_width = 20.0f;
	drawer_.SetLineWidth(line_width);
	quad_drawer_.SetLineWidth(line_width);

	if (!drawer_.Create(vertices, indices)) return false;
	if (!quad_drawer_.Create({{-1.0f, 0.0f}, {0.0f, 0.0f}})) return false;

	// Set initial OpenGL states
	::SetInitialStates();

	return true;
}
void Application::Unload()
{
	drawer_.Destroy();
	quad_drawer_.Destroy();
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

	quad_drawer_.Render();
	drawer_.Render();
}
InputController* Application::controller()
{
	return &controller_;
}

} // namespace poly