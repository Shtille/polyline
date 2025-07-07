#pragma once

#include "types.h"
#include "viewport.h"
#include "drawer/polyline_drawer.h"

namespace poly {

class Application {
public:
	Application();

	bool Load();
	void Unload();
	void Render();

	void SetViewport(int width, int height);
	void SetCapStyle(CapStyle cap_style);
	void SetJoinStyle(JoinStyle join_style);

	int width() const;
	int height() const;
	float aspect_ratio() const;
	CapStyle cap_style() const;
	JoinStyle join_style() const;

private:
	Viewport viewport_;
	PointArray points_;
	PolylineDrawer drawer_;
};

} // namespace poly