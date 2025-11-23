#pragma once

#include <array>

#include "base_app.h"
#include "camera.h"
#include "orbit_controller.h"
#include "drawer/square_drawer.h"
#include "drawer/screen_drawer.h"

namespace poly {

class Application : public BaseApplication {
public:
	Application();

	bool Load() override;
	void Unload() override;

	void Update() override;
	void Render() override;

	InputController* controller();

private:
	Camera camera_;
	OrbitController controller_;
	SquareDrawer square_drawer_;
	ScreenDrawer screen_drawer_;
	std::array<float, 4> background_color_;
	uint32_t framebuffer_;
	uint32_t color_texture_;
	uint32_t depth_renderbuffer_;
	uint32_t framebuffer_width_;
	uint32_t framebuffer_height_;
};

} // namespace poly