#pragma once

#include <array>

#include "base_app.h"
#include "camera.h"
#include "orbit_controller.h"
#include "drawer/wireframe_drawer.h"

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
	WireframeDrawer drawer_;
	std::array<float, 4> background_color_;
};

} // namespace poly