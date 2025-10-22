#pragma once
#include <string>
#include <GLM/glm.hpp>

class Shader
{
public:
	unsigned int id;

	Shader(const char* vtx_path, const char* frg_path);
	~Shader();

	void use() const;

	// uniforms !! -> int, float, mat4, vec3, vec4, sampler2D:
	void set_int_uniform(const char* name, int value) const;
	void set_float_uniform(const char* name, float value) const;
	void set_mat4_uniform(const char* name, glm::mat4 value) const;
	void set_vec3_uniform(const char* name, glm::vec3 value) const;
	void set_vec4_uniform(const char* name, glm::vec4 value) const;
	void set_sampler2d_uniform(const char* name, int texture_unit) const;
};

