#include "Mesh.hpp"
#include <IMGUI/imgui.h>
#include "GameObject.hpp"
#include <filesystem>
#include "ResourceManager.hpp"
#include <iostream>
#include "VAOFactory.hpp"
#include <GLAD/include/glad.h>

extern ResourceManager* rm;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> texturas) : 
	vertices(vertices), indices(indices), texturas(texturas)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::tex_coords));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normals));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
};

void Mesh::draw_mesh(Shader* sh)
{
	// cargar los samplers del shader:
	for (int i = 0; i < texturas.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0+i);

		sh->set_sampler2d_uniform(("material.texturas["+std::to_string(i)+"]").c_str(), i);

		glBindTexture(GL_TEXTURE_2D, texturas[i].id);
	}
	sh->set_int_uniform("material.count", texturas.size());

	// lo reseteamos:
	glActiveTexture(GL_TEXTURE0);

	// y dibujamos:
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/*
void Mesh::update(float& elapsed_time, std::vector<Instruction>& instrucciones)
{
	// simplemente cuadrar las instrucciones, y ejecutarlas cada frame
}*/

void Mesh::draw_header(int num)
{
	ImGui::PushID(("##mesh"+std::to_string(num)).c_str());
	if (ImGui::CollapsingHeader(("Mesh " + std::to_string(num)).c_str()))
	{
		ImGui::Text(("Indices: " + std::to_string(indices.size())).c_str());
		ImGui::Text(("VAO: " + std::to_string(VAO)).c_str());

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Texturas:");
		static std::vector<std::string> rutas_tex = { };
		rutas_tex.clear();
		rutas_tex.push_back("default");

		// guardar en el vector las rutas de las texturas de la carpeta:
		for (const auto& file : std::filesystem::directory_iterator("./textures/"))
		{
			if (file.path().extension().string() == ".jpg" || file.path().extension().string() == ".png")
			{
				rutas_tex.push_back(file.path().filename().string());
			}
		}

		for (int i = 0; i < texturas.size(); ++i)
		{
			if (ImGui::CollapsingHeader(("Textura " + std::to_string(i+1)).c_str()))
			{
				ImGui::Text(("ID: " + std::to_string(texturas[i].id)).c_str());

				ImGui::Text("Textura:");
				if (ImGui::BeginCombo(("##tex"+std::to_string(i)).c_str(), texturas[i].path.c_str()))
				{
					for (int j = 0; j < rutas_tex.size(); ++j)
					{
						if (ImGui::Selectable((rutas_tex[j]+ "##tex" + std::to_string(i)).c_str(), rutas_tex[j] == texturas[i].path))
						{
							texturas[i].path = rutas_tex[j];

							// en el caso de seleccionar "default":
							if (texturas[i].path == "default")
							{
								texturas[i].id = rm->default_texture;
								break;
							}

							// cargar la textura y bindearla:
							int texID = (intptr_t)rm->load_resource(RES_TYPE::TEXTURE, "./textures/"+texturas[i].path);

							if (texID == 0)
							{
								texturas[i].id = rm->default_texture;
								break;
							}

							texturas[i].id = texID;
							break;
						}
					}

					ImGui::EndCombo();
				}
				ImGui::Separator();
			}

		}
	}

	ImGui::PopID();

	/*
	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Spacing();

		// inicial
		std::vector<std::string> rutas_tex = { "default" };

		// guardar en el vector las rutas de las texturas de la carpeta:
		for (const auto& file : std::filesystem::directory_iterator("./textures/"))
		{
			if (file.path().extension().string() == ".jpg" || file.path().extension().string() == ".png")
			{
				rutas_tex.push_back(file.path().filename().string());
			}
		}

		ImGui::Text("Textura:");
		ImGui::SameLine();

		// mostrarlas en un combo:
		if (ImGui::BeginCombo("##tex", texture_name.c_str()))
		{
			for (int i = 0; i < rutas_tex.size(); ++i)
			{
				if (ImGui::Selectable(rutas_tex[i].c_str(), rutas_tex[i] == texture_name))
				{
					texture_name = rutas_tex[i];

					// en el caso de seleccionar "default":
					if (texture_name == "default")
					{
						textureID = rm->default_texture;
						break;
					}

					// cargar la textura y bindearla:
					int texID = (intptr_t)rm->load_resource(RES_TYPE::TEXTURE, "./textures/" + texture_name);

					if (texID == 0)
					{
						textureID = rm->default_texture;
						break;
					}

					textureID = texID;
					break;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Separator();
	}
	*/
}

/*
nlohmann::json Mesh::serialize()
{
	
	nlohmann::json mesh_comp;

	mesh_comp["texture_name"] = texture_name;
	mesh_comp["index_count"] = index_count;

	return mesh_comp;
	return nlohmann::json("cock");
}*/

/*
void Mesh::deserialize(const nlohmann::json& comp_nodo)
{
	
	texture_name = comp_nodo["texture_name"].get<std::string>();
	int texID = (intptr_t)rm->load_resource(RES_TYPE::TEXTURE, "./textures/"+texture_name);
	textureID = texID;

	VAO = create_default_cube_vao();

	index_count = comp_nodo["index_count"].get<unsigned int>();
}*/