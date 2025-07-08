#pragma once

#include "base_app.h"
#include "drawer/wireframe_drawer.h"

namespace poly {

class Application : public BaseApplication {
public:
	Application();

	bool Load() override;
	void Unload() override;
	void Render() override;

private:
	WireframeDrawer drawer_;
};

} // namespace poly