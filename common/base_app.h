#pragma once

#include "types.h"
#include "viewport.h"

namespace poly {

class BaseApplication {
public:
	BaseApplication();
	virtual ~BaseApplication() = default;

	virtual bool Load();
	virtual void Unload();

	virtual void Update();
	virtual void Render();

	void SetViewport(int width, int height);

	int width() const;
	int height() const;
	float aspect_ratio() const;

protected:
	Viewport viewport_;
};

} // namespace poly