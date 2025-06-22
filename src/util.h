#pragma once

#include <glad/glad.h>

namespace poly {

	bool LoadShaders(const char* vertex_file, const char* fragment_file, const char* geometry_file, GLuint& program);

} // namespace poly