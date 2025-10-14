#pragma once

#include "base_app.h"
#include "drawer/solid_polyline_drawer.h"
#include "drawer/polyline_drawer.h"

namespace poly {

class Application : public BaseApplication {
public:
	Application();

	bool Load() override;
	void Unload() override;
	void Render() override;

	void SetCapStyle(CapStyle cap_style);
	void SetJoinStyle(JoinStyle join_style);
	void SetShow(bool show);

	CapStyle cap_style() const;
	JoinStyle join_style() const;
	bool show() const;

private:
	PointArray points_;
	SolidPolylineDrawer solid_drawer_;
	PolylineDrawer drawer_;
	bool show_;
};

} // namespace poly