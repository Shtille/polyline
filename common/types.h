#pragma once

#include <vector>
#include <array>
#include <cstdint>

namespace poly {

typedef std::array<float, 2> Point;
typedef std::vector<Point> PointArray;

typedef std::array<float, 3> Point3D;
typedef std::vector<Point3D> Point3DArray;

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