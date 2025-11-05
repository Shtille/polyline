#pragma once

#include <vector>
#include <array>
#include <cstdint>

namespace poly {

typedef std::array<float, 2> Point;
typedef std::vector<Point> PointArray;

typedef std::array<float, 3> Point3D;
typedef std::vector<Point3D> Point3DArray;

typedef std::array<float, 4> Color;

typedef std::vector<uint32_t> IndicesArray;

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