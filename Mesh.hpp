#pragma once
#include "Component.hpp"
#include <string>
#include "Shader.hpp"
#include <GLM/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 tex_coords;
	glm::vec3 normals;
};

struct Texture
{
	unsigned int id;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> texturas;

	unsigned int VAO, VBO, EBO;

	/*
	unsigned int index_count;
	int textureID;
	unsigned int VAO;
	std::string texture_name; */

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> texturas);
	~Mesh() = default;

	void draw_mesh(Shader* sh);

	// aunque no sea un componente se siguen necesitando:
	void draw_header(int num);


	// void update(float& elapsed_time, std::vector<Instruction>& instrucciones);

	// nlohmann::json serialize();

	// void deserialize(const nlohmann::json& comp_nodo);
};