#pragma once
#include <string>
#include "Mesh.hpp"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Component
{
public:
	std::string name;
	std::vector<Mesh> meshes;
	bool success;

	Model(const char* path);
	~Model() = default;

	void draw_model(Shader* sh);
	void draw_header();

	void process_node(aiNode* nodo, const aiScene* model);
	void process_mesh(aiMesh* mesh, const aiScene* model);

	bool load_model(const char* path);

	void update(float& elapsed_time, std::vector<Instruction>& instrucciones) override;

	nlohmann::json serialize() override;

	void deserialize(const nlohmann::json& comp_nodo) override;
};