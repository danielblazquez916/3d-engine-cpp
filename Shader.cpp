#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <GLAD/include/glad.h>
#include <GLM/gtc/type_ptr.hpp>
#include "DebugGUI.hpp"
#include "Constants.hpp"

extern DebugGUI* debug_window;

Shader::~Shader()
{
	if(id != 0) glDeleteProgram(id);
}

Shader::Shader(const char* vtx_path, const char* frg_path) : id(0)
{
	// 1º) leer la informacion de los ficheros shader:
	std::ifstream vtx_shader_ifs;
	std::ifstream frg_shader_ifs;

	std::string vtx_shader_s;
	std::string frg_shader_s;

	vtx_shader_ifs.exceptions(std::ios::failbit | std::ios::badbit);
	frg_shader_ifs.exceptions(std::ios::failbit | std::ios::badbit);

	try
	{
		vtx_shader_ifs.open(vtx_path);
		frg_shader_ifs.open(frg_path);

		std::stringstream vtx_shader_ss, frg_shader_ss;

		vtx_shader_ss << vtx_shader_ifs.rdbuf();
		frg_shader_ss << frg_shader_ifs.rdbuf();

		vtx_shader_s = vtx_shader_ss.str();
		frg_shader_s = frg_shader_ss.str();
	}
	catch (std::ios::failure& ex)
	{
		ImVec4* color = new ImVec4(1.f, 0.f, 0.f, 1.f);
		debug_window->LOG((std::string("[Shader] ~ Error en la lectura del shader, error: \n") + ex.what()).c_str(), color);
	}

	// 2º) Compilar el codigo fuente, enlazar, y generar el programa shader:
	const char* vtx_shader_src = vtx_shader_s.c_str();
	const char* frg_shader_src = frg_shader_s.c_str();

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vtx_shader_src, 0);
	glCompileShader(vertex_shader);

	int success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		// obtener el error completo:
		char buffer[512];
		int length = 0;
		std::string error = "";
		glGetShaderInfoLog(vertex_shader, sizeof(buffer), &length, buffer);
		for (int i = 0; i < length; ++i)
		{
			error += buffer[i];
		}

		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG(
			(std::string("[Shader] ~ Ha ocurrido un error en la compilacion del Vertex Shader.\n")+
			"Error especifico: " + error).c_str(),
			color
		);
		return;
	}

	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &frg_shader_src, 0);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (success == 0)
	{
		// obtener el error completo:
		char buffer[512];
		int length = 0;
		std::string error = "";
		glGetShaderInfoLog(fragment_shader, sizeof(buffer), &length, buffer);
		for (int i = 0; i < length; ++i)
		{
			error += buffer[i];
		}

		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG(
			(std::string("[Shader] ~ Ha ocurrido un error en la compilacion del Fragment Shader.\n") +
				"Error especifico: " + error).c_str(),
			color
		);
		return;
	}

	id = glCreateProgram();
	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);
	
	glLinkProgram(id);

	// 3º) eliminar el codigo objeto (ya no sirve, solo nos sirve el programa resultante):
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use() const
{
	glUseProgram(id);
}

// uniforms !!
void Shader::set_int_uniform(const char* name, int value) const
{
	use();
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::set_float_uniform(const char* name, float value) const
{
	use();
	glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::set_mat4_uniform(const char* name, glm::mat4 value) const
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, 0, glm::value_ptr(value));
}

void Shader::set_vec3_uniform(const char* name, glm::vec3 value) const
{
	use();
	glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void Shader::set_vec4_uniform(const char* name, glm::vec4 value) const
{
	use();
	glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void Shader::set_sampler2d_uniform(const char* name, int texture_unit) const
{
	use();
	glUniform1i(glGetUniformLocation(id, name), texture_unit);
}