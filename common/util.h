#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>

namespace poly {

	bool LoadShaders(const char* vertex_file, const char* fragment_file, const char* geometry_file, GLuint& program);
	void MakeMinimum(const glm::vec3& pos, glm::vec3& min_pos);
	void MakeMaximum(const glm::vec3& pos, glm::vec3& max_pos);
	float MinimumComponent(const glm::vec3& pos);
	float MaximumComponent(const glm::vec3& pos);

} // namespace poly