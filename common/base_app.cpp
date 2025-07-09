#include "base_app.h"

#include <glad/glad.h>

namespace poly {

BaseApplication::BaseApplication()
: viewport_()
{
	SetViewport(800, 600);
}
bool BaseApplication::Load()
{
	return true;
}
void BaseApplication::Unload()
{
}
void BaseApplication::Update()
{
}
void BaseApplication::Render()
{
}
void BaseApplication::SetViewport(int width, int height)
{
	viewport_.width = width;
	viewport_.height = height;
	viewport_.aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
}
int BaseApplication::width() const
{
	return viewport_.width;
}
int BaseApplication::height() const
{
	return viewport_.height;
}
float BaseApplication::aspect_ratio() const
{
	return viewport_.aspect_ratio;
}

} // namespace poly