#pragma once

#include "base_app.h"
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

	CapStyle cap_style() const;
	JoinStyle join_style() const;

private:
	PointArray points_;
	PolylineDrawer drawer_;
};

} // namespace poly