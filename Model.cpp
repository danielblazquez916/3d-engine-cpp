#include "Model.hpp"
#include <iostream>
#include "ResourceManager.hpp"
#include "DebugGUI.hpp"
#include "Constants.hpp"
#include <GLAD/include/glad.h>
#include <future>

extern ResourceManager* rm;
extern DebugGUI* debug_window;

Model::Model(const char* path) : Component("Model"), success(1)
{
	if (!load_model(path))
	{
		success = 0;
		load_model("default");
	}
}

bool Model::load_model(const char* path)
{
	if (path == nullptr)
	{
		return 0; // y me la pela
	}

	if (path == "default")
	{
		std::vector<Vertex> cube_attrbs = {
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
		std::vector<unsigned int> cube_index = {
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

		// crear un cubo por defecto:
		Mesh mesh(cube_attrbs, cube_index, { {(unsigned int)rm->default_texture, std::string(path)} });
		meshes.clear();
		meshes.push_back(mesh);
		name = path;
		return 1;
	}

	// cargamos el modelo con el path
	Assimp::Importer imp;
	const aiScene* model_obj = imp.ReadFile("./models/" + std::string(path), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!model_obj || model_obj->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !model_obj->mRootNode)
	{
		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG((std::string("[Assimp] ~ ") + imp.GetErrorString()).c_str(), color);

		return 0;
	}
	name = path;

	process_node(model_obj->mRootNode, model_obj);
	return 1;
}

void Model::process_node(aiNode* nodo, const aiScene* model)
{
	for (int i = 0; i < nodo->mNumMeshes; ++i)
	{
		aiMesh* ai_mesh = model->mMeshes[nodo->mMeshes[i]];
		process_mesh(ai_mesh, model);
	}

	for (int i = 0; i < nodo->mNumChildren; ++i)
	{
		process_node(nodo->mChildren[i], model);
	}
}

void Model::process_mesh(aiMesh* mesh, const aiScene* model)
{
	// procesar vertices
	std::vector<Vertex> vertices;
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex v;

		v.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		v.normals = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		v.tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

		vertices.push_back(v);
	}

	// procesar indices
	std::vector<unsigned int> indices;
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// procesar texturas
	std::vector<Texture> texturas;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* ai_mat = model->mMaterials[mesh->mMaterialIndex];

		for (int i = 0; i < ai_mat->GetTextureCount(aiTextureType_DIFFUSE); ++i)
		{
			aiString path;
			ai_mat->GetTexture(aiTextureType_DIFFUSE, i, &path);

			Texture tex;

			if(path.C_Str() == "default") tex.id = rm->default_texture;
			else tex.id = (uintptr_t) rm->load_resource(RES_TYPE::TEXTURE, std::string("./textures/") + path.C_Str());

			if (tex.id == 0) tex.id = rm->default_texture;
			tex.path = path.data;

			texturas.push_back(tex);
		}
	}

	meshes.push_back(Mesh(vertices, indices, texturas));
}

void Model::update(float& elapsed_time, std::vector<Instruction>& instrucciones)
{
}

nlohmann::json Model::serialize()
{
	nlohmann::json model_node;

	nlohmann::json meshes_node;
	for (auto& mesh : meshes)
	{
		nlohmann::json mesh_node;

		// serializamos texturas:
		nlohmann::json texturas_node;
		for (auto& tex : mesh.texturas)
		{
			nlohmann::json tex_node;
			tex_node["path"] = tex.path;
			texturas_node.push_back(tex_node);
		}

		mesh_node["texturas"] = texturas_node;
		meshes_node.push_back(mesh_node);
	}

	model_node["model_path"] = name;
	model_node["meshes"] = meshes_node;

	return model_node;
}

void Model::deserialize(const nlohmann::json& comp_nodo)
{
	std::string model_path = comp_nodo["model_path"].get<std::string>();

	Model* m = (model_path == "default") ? new Model("default") : (Model*)rm->load_resource(RES_TYPE::MODEL, comp_nodo["model_path"].get<std::string>());
	if (m == nullptr) return; // error de que no se encuentra la ruta del modelo.

	for (int i = 0; i < comp_nodo["meshes"].size(); ++i)
	{
		nlohmann::json meshes_node = comp_nodo["meshes"];

		for (int j = 0; j < meshes_node[i]["texturas"].size(); ++j)
		{
			nlohmann::json texturas_node = meshes_node[i]["texturas"];
			std::string path = texturas_node[j]["path"].get<std::string>();

			Texture t;
			t.id = (path == "default") ? rm->default_texture : (uintptr_t) rm->load_resource(RES_TYPE::TEXTURE, "./textures/" + path);
			t.path = path;

			m->meshes[i].texturas[j] = t;
		}
	}

	name = m->name;
	meshes = m->meshes;

	if (model_path == "default")
	{
		delete m;
		m = nullptr;
	}
}

void Model::draw_model(Shader* sh)
{
	for (auto& mesh : meshes)
	{
		mesh.draw_mesh(sh);
	}
}

void Model::draw_header()
{
	if (ImGui::CollapsingHeader("Model"))
	{
		// informacion del modelo
		ImGui::Text("Modelos para usar:");
		ImGui::Separator();

		static std::vector<std::string> models_from_dir;
		models_from_dir.clear();
		models_from_dir.push_back("default");

		// guardar en el vector las rutas de las texturas de la carpeta:
		for (const auto& file : std::filesystem::directory_iterator("./models/"))
		{
			if (file.path().extension().string() == ".obj" || file.path().extension().string() == ".blend" || file.path().extension().string() == ".gltf")
			{
				models_from_dir.push_back(file.path().filename().string());
			}
		}

		if (ImGui::BeginCombo("##models", name.c_str()))
		{
			// iterar a traves de los componentes y agarrar sus nombres
			for (int i = 0; i < models_from_dir.size(); ++i)
			{
				if (ImGui::Selectable(models_from_dir[i].c_str(), models_from_dir[i] == name))
				{
					name = models_from_dir[i];
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("usar"))
		{
			if (name == "default") load_model("default");
			else
			{
				Model* m = (Model*)rm->load_resource(RES_TYPE::MODEL, name);

				if (m != nullptr)
				{
					meshes.clear();
					this->meshes = m->meshes;
				}
			}
		}

		ImGui::Spacing();

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Meshes:");
		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].draw_header(i + 1);
		}
	}
}

