#pragma once

#include "types.h"
#include "viewport.h"
#include "drawer.h"

namespace poly {

class Application {
public:
	Application();

	bool Load();
	void Unload();
	void Render();

	void SetViewport(int width, int height);

	int width() const;
	int height() const;
	float aspect_ratio() const;

private:
	Viewport viewport_;
	PointArray points_;
	Drawer drawer_;
};

} // namespace poly