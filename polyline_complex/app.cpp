#include "app.h"

#include <glad/glad.h>

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
, square_drawer_(&viewport_)
, screen_drawer_()
, background_color_({0.0f, 0.0f, 0.0f, 1.0f})
, framebuffer_(0)
, color_texture_(0)
, depth_renderbuffer_(0)
, framebuffer_width_(0)
, framebuffer_height_(0)
{
}
bool Application::Load()
{
	// Create squares
	Color color = {1.0f, 1.0f, 1.0f, 0.5f};
	if (!square_drawer_.Create({
		{{0.0f, 0.0f}, 0.1f, color}, 
		{{0.1f, 0.1f}, 0.1f, color},
	}))
		return false;

	// Create screen quad
	if (!screen_drawer_.Create()) return false;

	// Set initial OpenGL states
	::SetInitialStates();

	framebuffer_width_ = viewport_.width;
	framebuffer_height_ = viewport_.height;

	// Create frambuffer
	glGenFramebuffers(1, &framebuffer_);

	// Create color texture
	glGenTextures(1, &color_texture_);
	glBindTexture(GL_TEXTURE_2D, color_texture_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, framebuffer_width_, framebuffer_height_, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create depth renderbuffer
	glGenRenderbuffers(1, &depth_renderbuffer_);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, framebuffer_width_, framebuffer_height_); // GL_DEPTH24_STENCIL8

	// Attach targets to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture_, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer_);

	const GLenum color_targets[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, color_targets);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	screen_drawer_.set_texture(color_texture_);

	return true;
}
void Application::Unload()
{
	square_drawer_.Destroy();
	screen_drawer_.Destroy();

	if (framebuffer_ != 0)
	{
		glDeleteFramebuffers(1, &framebuffer_);
		framebuffer_ = 0;
	}
	if (color_texture_ != 0)
	{
		glDeleteTextures(1, &color_texture_);
		color_texture_ = 0;
	}
	if (depth_renderbuffer_ != 0)
	{
		glDeleteRenderbuffers(1, &depth_renderbuffer_);
		depth_renderbuffer_ = 0;
	}
}
void Application::Update()
{
	camera_.Update();
}
void Application::Render()
{
	// Render to texture
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
	glViewport(0, 0, framebuffer_width_, framebuffer_height_);
	glClearColor(
		background_color_[0], 
		background_color_[1],
		background_color_[2],
		background_color_[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	square_drawer_.Render();

	// Draw fullscreen quad with texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, viewport_.width, viewport_.height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	screen_drawer_.Render();
}
InputController* Application::controller()
{
	return &controller_;
}

} // namespace poly