#include "app.h"

#include <glad/glad.h>

namespace poly {

Application::Application()
: BaseApplication()
, points_()
, solid_drawer_(&viewport_)
, drawer_(&viewport_)
, show_(true)
{
}
bool Application::Load()
{
	points_ = {
		{-0.5f,  0.0f},
		{ 0.0f,  0.5f},
		{ 0.0f, -0.5f},
		{ 0.5f,  0.0f},
	};

	if (!solid_drawer_.Create(points_)) return false;
	if (!drawer_.Create(points_)) return false;

	return true;
}
void Application::Unload()
{
	solid_drawer_.Destroy();
	drawer_.Destroy();
}
void Application::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	solid_drawer_.Render();
	if (show_)
		drawer_.Render();
}
void Application::SetCapStyle(CapStyle cap_style)
{
	solid_drawer_.SetCapStyle(cap_style);
}
void Application::SetJoinStyle(JoinStyle join_style)
{
	solid_drawer_.SetJoinStyle(join_style);
}
void Application::SetShow(bool show)
{
	show_ = show;
}
CapStyle Application::cap_style() const
{
	return solid_drawer_.cap_style();
}
JoinStyle Application::join_style() const
{
	return solid_drawer_.join_style();
}
bool Application::show() const
{
	return show_;
}

} // namespace poly