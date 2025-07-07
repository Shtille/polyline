#pragma once

#include <vector>
#include <array>
#include <cstdint>

namespace poly {

typedef std::array<float, 2> Point;
typedef std::vector<Point> PointArray;

enum class CapStyle {
	kFlat,
	kSquare,
	kRound
};

enum class JoinStyle {
	kBevel,
	kMiter,
	kRound
};

} // namespace poly