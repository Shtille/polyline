#include "app.h"

#include <glad/glad.h>

namespace poly {

Application::Application()
: viewport_()
, points_()
, drawer_(&viewport_)
{
	SetViewport(800, 600);
}
bool Application::Load()
{
	points_ = {
		{-0.5f,  0.0f},
		{ 0.0f,  0.5f},
		{ 0.0f, -0.5f},
		{ 0.5f,  0.0f}
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
void Application::SetViewport(int width, int height)
{
	viewport_.width = width;
	viewport_.height = height;
	viewport_.aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
}
void Application::SetCapStyle(CapStyle cap_style)
{
	drawer_.SetCapStyle(cap_style);
}
void Application::SetJoinStyle(JoinStyle join_style)
{
	drawer_.SetJoinStyle(join_style);
}
int Application::width() const
{
	return viewport_.width;
}
int Application::height() const
{
	return viewport_.height;
}
float Application::aspect_ratio() const
{
	return viewport_.aspect_ratio;
}
CapStyle Application::cap_style() const
{
	return drawer_.cap_style();
}
JoinStyle Application::join_style() const
{
	return drawer_.join_style();
}

} // namespace poly