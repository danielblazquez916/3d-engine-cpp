#include "VAOFactory.hpp"
#include <vector>
#include <GLAD/include/glad.h>

// realmente no tiene tanta utilidad, es solo para hacerlo mas legible :)
/*
struct Vertice
{
	float pos[3];
	float texture_coords[2];
	float normals[3];
};*/

unsigned int create_skybox_vao()
{
	std::vector<float> skybox_attrb = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int vbo, vao;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skybox_attrb.size(), skybox_attrb.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return vao;
}
/*
unsigned int create_default_cube_vao()
{
	// Atributos de vertice por default (pos local, coord de textura y normales para calculos)
	std::vector<Vertice> cube_attrbs = {
		// Front face
		{{-1.f, -1.f,  1.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{ 1.f, -1.f,  1.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
		{{ 1.f,  1.f,  1.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},
		{{-1.f,  1.f,  1.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}},

		// Back face
		{{-1.f, -1.f, -1.f}, {1.f, 0.f}, {0.f, 0.f, -1.f}},
		{{ 1.f, -1.f, -1.f}, {0.f, 0.f}, {0.f, 0.f, -1.f}},
		{{ 1.f,  1.f, -1.f}, {0.f, 1.f}, {0.f, 0.f, -1.f}},
		{{-1.f,  1.f, -1.f}, {1.f, 1.f}, {0.f, 0.f, -1.f}},

		// Left face
		{{-1.f, -1.f, -1.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-1.f, -1.f,  1.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{-1.f,  1.f,  1.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{-1.f,  1.f, -1.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f}},

		// Right face
		{{ 1.f, -1.f, -1.f}, {1.f, 0.f}, {1.f, 0.f, 0.f}},
		{{ 1.f, -1.f,  1.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{ 1.f,  1.f,  1.f}, {0.f, 1.f}, {1.f, 0.f, 0.f}},
		{{ 1.f,  1.f, -1.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},

		// Top face
		{{-1.f,  1.f, -1.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
		{{ 1.f,  1.f, -1.f}, {1.f, 0.f}, {0.f, 1.f, 0.f}},
		{{ 1.f,  1.f,  1.f}, {1.f, 1.f}, {0.f, 1.f, 0.f}},
		{{-1.f,  1.f,  1.f}, {0.f, 1.f}, {0.f, 1.f, 0.f}},

		// Bottom face
		{{-1.f, -1.f, -1.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}},
		{{ 1.f, -1.f, -1.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{ 1.f, -1.f,  1.f}, {0.f, 1.f}, {0.f, -1.f, 0.f}},
		{{-1.f, -1.f,  1.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},
	};

	// Indices por default para crear un cubo!
	unsigned int cube_index[] = {
		// Front face
		0, 1, 2,   2, 3, 0,
		// Back face
		4, 5, 6,   6, 7, 4,
		// Left face
		8, 9, 10,  10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};

	unsigned int vbo, ebo, vao;

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// cargamos en el vertex buffer de la VRAM la información:
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * cube_attrbs.size(), cube_attrbs.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index), cube_index, GL_STATIC_DRAW);

	// Le decimos como tiene que interpretarlo y los enlazamos con los atributos de entrada de los shaders:
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertice), (void*)offsetof(Vertice, Vertice::pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(Vertice), (void*)offsetof(Vertice, Vertice::texture_coords));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, 0, sizeof(Vertice), (void*)offsetof(Vertice, Vertice::normals));
	glEnableVertexAttribArray(2);

	return vao;
}
*/