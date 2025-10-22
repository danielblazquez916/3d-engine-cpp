#include "SceneGUI.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include <IMGUI/imgui.h>
#include <GLAD/include/glad.h>
#include "VAOFactory.hpp"
#include <vector>
#include "DebugGUI.hpp"
#include <GLFW/glfw3.h>
#include "Constants.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

GameObject* selected_object_ptr = nullptr; // en principio no hay ninguno seleccionado.
extern Camera* active_camera;

// variables globales:
extern std::vector<GameObject*> gameobjects;
extern DebugGUI* debug_window;
extern ResourceManager* rm;

extern std::string actual_scene;

int id_counting = 1;

// NECESITO ALMACENAR LOS GAMEOBJECTS EN LA HEAP PORQUE
// SI NO LO HICIERA TENDRÍA QUE HACER UNA COPIA TEMPORAL
// EN LA STACK PARA DESPUES ALMACENARLO EN EL VECTOR, LO QUE
// SUPONE QUE LLAME AL DESTRUCTOR YA QUE SE ESTA LIBERANDO
// DE LA STACK. NO PUEDE LLAMAR AL DESTRUCTOR PORQUE SI LO HACE
// SE CARGA LOS COMPONENTES QUE SE DEBEN LIBERAR CUANDO ESE
// OBJETO SE ELIMINE CUANDO TOCA. CONCLUSIÓN: GENTE, TENGO
// QUE COBRAR UNA ENTRADA.


// GUARDAR EL ID COUNTING EN EL SERIALIZE PARA CARGARLO TAMBIEN:
void SceneGUI::add_gameobject(const std::string& obj_name) const
{
	// primero se añade (sin crear copia)
	GameObject* obj = new GameObject(id_counting, obj_name);
	++id_counting;

	// Mesh* mesh = (Mesh*) obj->get_component_by_name("Mesh");
	
	// crear la textura default:
	/*
	mesh->textureID = rm->default_texture;
	mesh->VAO = create_default_cube_vao();
	mesh->index_count = 36;*/

	gameobjects.push_back(obj);
}

void SceneGUI::render(int& fps, bool& vsync) const
{
	if (ImGui::Begin("Escena"))
	{
		// ver la escena actual:
		if(actual_scene.empty()) ImGui::Text("Escena cargada: (Ninguna)");
		else
		{
			// buscar si sigue existiendo:
			bool scene_exists = 0;
			static bool stop = 0;
			for (const auto& scene : std::filesystem::directory_iterator("./scenes/"))
			{
				if (scene.path().filename().string() == actual_scene)
				{
					scene_exists = 1;
					stop = 0;
				}
			}

			if (!scene_exists && !stop)
			{
				ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]);
				debug_window->LOG("[SceneManager] ~ La escena que estas ejecutando parece que ya no existe en la carpeta, guarda para no perder tu progreso!", color);

				actual_scene = "";
				stop = 1;
			}

			ImGui::Text(("Escena cargada: \"" + actual_scene + "\"").c_str());
		}

		std::string fps_t = "FPS: " + std::to_string(fps);
		ImGui::Text(fps_t.c_str());

		ImGui::Text("VSync: ");
		ImGui::SameLine();
		if (ImGui::Checkbox("##vsync", &vsync))
		{
			glfwSwapInterval(vsync);

			if (vsync == 0)
			{
				ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]); // mas anaranjado para warnings
				debug_window->LOG("[Scene] ~ Tenga cuidado con desactivar la sincronizacion vertical, su tarjeta grafica puede estar trabajando de mas.", color);
			}
		}

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Crear objeto"))
		{
			// metodo para crear un nuevo objeto.
			add_gameobject("nuevo objeto");
			// debug_window.LOG((std::string("[Scene] ~ Has creado un objeto! Hay ") + std::to_string(gameobjects.size()) + std::string(" en total!")).c_str(), nullptr);
		}

		ImGui::Separator();
		ImGui::Spacing();

		for (auto it = gameobjects.begin(); it != gameobjects.end(); ++it)
		{
			// En ImGUI, cuando generamos interfaz dinamicamente, cada elemento 
			// debería tener un id unico para evitar colisiones:
			ImGui::PushID((*it)->id);
			
			if (active_camera != nullptr && (*it)->id == active_camera->owner->id)
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.f, 1.f, 0.f, 0.5f));

			if (ImGui::Selectable((*it)->name.c_str(), selected_object_ptr == *it))
			{
				selected_object_ptr = *it;
			}

			bool can_i_open_my_popup = 0;

			// al hacer click derecho, que se pueda eliminar:
			if (ImGui::BeginPopupContextItem())
			{
				char buffer[20] = "";
				
				ImGui::Text("Rename: ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(120.f);
				if (ImGui::InputText("##edit", &buffer[0], 20, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					// comprobar que no existe el nombre:
					for (GameObject* o : gameobjects)
					{
						if (o->name == buffer)
						{
							buffer[0] = '\0'; // limpio el buffer ya que esta repetido
							break;
						}
					}
					if(std::strlen(buffer) > 0) (*it)->name = buffer; // solo si no se ha borrado
				}

				if (ImGui::MenuItem("Deseleccionar"))
				{
					selected_object_ptr = nullptr;
				}

				if (ImGui::MenuItem("Eliminar objeto"))
				{
					can_i_open_my_popup = 1;
				}

				ImGui::EndPopup();
			}

			if (can_i_open_my_popup) ImGui::OpenPopup("Confirmar eliminacion");

			if (ImGui::BeginPopupModal("Confirmar eliminacion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Seguro que lo quieres eliminar? No volvera!!");

				ImGui::Separator();
				ImGui::Spacing();

				if (ImGui::Button("Sip"))
				{
					// eliminamos el objeto seleccionado:
					delete *it;
					gameobjects.erase(it);
					selected_object_ptr = nullptr;

					ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
					ImGui::PopID();
					break;
				}

				ImGui::SameLine();

				if (ImGui::Button("Nop"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			ImGui::PopID();
			if (active_camera != nullptr && (*it)->id == active_camera->owner->id)
				ImGui::PopStyleColor(1);
		}

	}
	ImGui::End();
}







