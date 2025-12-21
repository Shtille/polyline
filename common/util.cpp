#include "util.h"

#include <glm/geometric.hpp> // glm::dot

#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

static bool ReadFile(const char* filename, std::string& data)
{
	FILE * f = fopen(filename, "rt");
	if (!f)
	{
		fprintf(stderr, "Can't open file '%s'\n", filename);
		return false;
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	if (size <= 0)
		return false;

	data.resize(size+1);
	fseek(f, 0, SEEK_SET);
	fread(&data[0], sizeof(char), size, f);
	data[size] = '\0';
	fclose(f);

	return true;
}

namespace poly {

	bool LoadShaders(const char* vertex_file, const char* fragment_file, const char* geometry_file, GLuint& program)
	{
		// Read our shaders into the appropriate buffers
		std::string vertexSource, geometrySource, fragmentSource;
		if (!ReadFile(vertex_file, vertexSource) ||
			geometry_file != nullptr && !ReadFile(geometry_file, geometrySource) ||
			!ReadFile(fragment_file, fragmentSource))
			return false;

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = (const GLchar *)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			fprintf(stderr, "Error during vertex shader '%s' compilation:\n", vertex_file);
			fprintf(stderr, "%s\n", &infoLog[0]);
			
			// In this simple program, we'll just leave
			return false;
		}

		GLuint geometryShader = 0;
		if (geometry_file != nullptr)
		{
			// Create an empty vertex shader handle
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar *source = (const GLchar *)geometrySource.c_str();
			glShaderSource(geometryShader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(geometryShader);

			GLint isCompiled = 0;
			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);
				
				// We don't need the shader anymore.
				glDeleteShader(geometryShader);
				glDeleteShader(vertexShader);

				// Use the infoLog as you see fit.
				fprintf(stderr, "Error during geometry shader '%s' compilation:\n", geometry_file);
				fprintf(stderr, "%s\n", &infoLog[0]);
				
				// In this simple program, we'll just leave
				return false;
			}
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);
			if (geometry_file != nullptr)
				glDeleteShader(geometryShader);

			// Use the infoLog as you see fit.
			fprintf(stderr, "Error during fragment shader '%s' compilation:\n", fragment_file);
			fprintf(stderr, "%s\n", &infoLog[0]);
			
			// In this simple program, we'll just leave
			return false;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		if (geometry_file != nullptr)
			glAttachShader(program, geometryShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			
			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			if (geometry_file != nullptr)
				glDeleteShader(geometryShader);

			// Use the infoLog as you see fit.
			fprintf(stderr, "Error during shader linkage:\n");
			fprintf(stderr, "%s\n", &infoLog[0]);
			
			// In this simple program, we'll just leave
			return false;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		if (geometry_file != nullptr)
			glDetachShader(program, geometryShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometry_file != nullptr)
			glDeleteShader(geometryShader);
		return true;
	}
	void MakeMinimum(const glm::vec3& pos, glm::vec3& min_pos)
	{
		if (min_pos.x > pos.x)
			min_pos.x = pos.x;
		if (min_pos.y > pos.y)
			min_pos.y = pos.y;
		if (min_pos.z > pos.z)
			min_pos.z = pos.z;
	}
	void MakeMaximum(const glm::vec3& pos, glm::vec3& max_pos)
	{
		if (max_pos.x < pos.x)
			max_pos.x = pos.x;
		if (max_pos.y < pos.y)
			max_pos.y = pos.y;
		if (max_pos.z < pos.z)
			max_pos.z = pos.z;
	}
	float MinimumComponent(const glm::vec3& pos)
	{
		float value = pos.x;
		if (value > pos.y)
			value = pos.y;
		if (value > pos.z)
			value = pos.z;
		return value;
	}
	float MaximumComponent(const glm::vec3& pos)
	{
		float value = pos.x;
		if (value < pos.y)
			value = pos.y;
		if (value < pos.z)
			value = pos.z;
		return value;
	}
	glm::vec3 GetBoundingSphereCenter(const glm::vec3& min, const glm::vec3& max)
	{
		return 0.5f * (min + max);
	}
	float GetBoundingSphereRadius(const glm::vec3& min, const glm::vec3& max)
	{
		glm::vec3 center = GetBoundingSphereCenter(min, max);

		// All 8 box points
		const glm::vec3 points[] = {
			glm::vec3(min.x, min.y, min.z),
			glm::vec3(min.x, min.y, max.z),
			glm::vec3(min.x, max.y, min.z),
			glm::vec3(min.x, max.y, max.z),

			glm::vec3(max.x, min.y, min.z),
			glm::vec3(max.x, min.y, max.z),
			glm::vec3(max.x, max.y, min.z),
			glm::vec3(max.x, max.y, max.z),
		};
		float max_size_sqr = 0.0f;
		for (auto& point : points)
		{
			glm::vec3 offset = point - center;
			float size_sqr = glm::dot(offset, offset);
			if (max_size_sqr < size_sqr)
				max_size_sqr = size_sqr;
		}
		return std::sqrtf(max_size_sqr);
	}

} // namespace poly