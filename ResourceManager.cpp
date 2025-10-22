#include "ResourceManager.hpp"
#include "Shader.hpp"
#include <GLAD/include/glad.h>
#include <iostream>
#include "DebugGUI.hpp"
#include <string>
#include "Model.hpp"
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern DebugGUI* debug_window;

ResourceManager::ResourceManager() : default_texture(0)
{ 
	stbi_set_flip_vertically_on_load(1);

	// generar la textura default:
	unsigned int teID;
	glGenTextures(1, &teID);
	glBindTexture(GL_TEXTURE_2D, teID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char def_img[16] =
	{
		255,255,0,255,
		0,0,0,255,
		0,0,0,255,
		255,255,0,255,
	};

	// cargamos los bytes en la VRAM:
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, def_img);

	glBindTexture(GL_TEXTURE_2D, 0);

	default_texture = teID;
};

void* ResourceManager::load_resource(RES_TYPE tipo, const std::string& path)
{
	// Generamos la clave para el hashmap:
	std::string clave = static_cast<int>(tipo) + ">" + path;

	// Buscar si ya existe en el caché:
	auto it = res_cache.find(clave);

	if (it != res_cache.end())
	{
		return it->second;
	}

	// En el caso de que no exista, lo cargamos desde disco:
	void* dato = load_external_resource(tipo, path);

	if (!dato)
	{
		ImVec4* color = new ImVec4(1.f, 0.f, 0.f, 1.f);
		debug_window->LOG((std::string("[ResourceManager] ~ Error al intentar importar: ") + path).c_str(), color);
		return nullptr;
	}

	res_cache[clave] = dato;

	debug_window->LOG((std::string("[ResourceManager] ~ Se ha cargado el recurso: ") + path).c_str(), nullptr);

	return dato;
}

void* ResourceManager::load_external_resource(RES_TYPE tipo, const std::string& path)
{
	switch (tipo)
	{
	case RES_TYPE::SHADER:
	{
		// obtengo el path real del vtx shader y el frag shader:
		std::string vtx_path = (path + ".vert");
		std::string frg_path = (path + ".frag");

		// almaceno el shader en la heap:
		Shader* sh = new Shader(vtx_path.c_str(), frg_path.c_str());
		if (sh->id == 0) return nullptr;

		return sh; // se convierte implicitamente;
	}

	case RES_TYPE::CUBEMAP:
	{
		stbi_set_flip_vertically_on_load(0);
		// ES IMPORTANTE QUE EL PATH EN EL CUBEMAP SEA UN DIRECTORIO!!

		// ruta de las imagenes de la carpeta elegida:
		std::vector<std::string> caras = {
			"right",
			"left",
			"top",
			"bottom",
			"front",
			"back"
		};
		
		unsigned int id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		int width, height, channels;
		for (unsigned int i = 0; i < caras.size(); ++i)
		{
			unsigned char* data = stbi_load((path + caras[i] + ".jpg").c_str(), &width, &height, &channels, 0);
			if (!data) // se tienen que importar TODAS las caras, si no, no se importa ninguna >:C
			{
				data = stbi_load((path + caras[i] + ".png").c_str(), &width, &height, &channels, 0);
				if (!data)
				{
					stbi_image_free(data);
					return nullptr;
				}
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return (void*)id;
	}

	case RES_TYPE::MODEL:
	{
		Model* m = new Model(path.c_str());
		if (!m->success)
		{
			delete m;
			m = nullptr; // no hace falta realmente
			return nullptr;
		}
		return m;
	}

	case RES_TYPE::TEXTURE:
	{
		stbi_set_flip_vertically_on_load(1);

		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// WRAPPING:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// FILTERING:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			stbi_image_free((void*)data);
			return nullptr;
		}

		GLenum format = GL_RGBA;
		if (channels < 4)
			format = GL_RGB;

		// cargamos los bytes a la VRAM y generamos el mipmap:
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free((void*)data);
		return (void*)texture; // CRIMEN
	}
	}

	return nullptr;
}