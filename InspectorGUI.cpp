#include "InspectorGUI.hpp"
#include <IMGUI/imgui.h>
#include "GameObject.hpp"
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include "ResourceManager.hpp"
#include <ctype.h>
#include <GLAD/include/glad.h>
#include <iostream>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Script.hpp"
#include "Camera.hpp"
#include "Model.hpp"

// variables globales:
extern GameObject* selected_object_ptr;
extern ResourceManager* rm;

// para saber los componentes creados:
// (en el futuro se añadiran automaticamente, espero...)
struct ComponentAdder
{
	const char* name;
	Component*(*return_comp)();
};

// esto de abajo sirve para poder crear una instancia en la heap
// del componente que me interese a partir del nombre del mismo;

// la clave del exito:
std::vector<ComponentAdder> components_to_add =
{
	{"Transform", []() -> Component* { return new Transform(); }}, 
	{"Model", []() -> Component* { return new Model("default"); }},
	{"Script", []() -> Component* { return new Script(); }},
	{"Camera", []() -> Component* { return new Camera(&*selected_object_ptr); }}
};

int component_selected_to_add = 0;

void InspectorGUI::render() const
{
	if (ImGui::Begin("Inspector"))
	{
		if (selected_object_ptr == nullptr)
		{
			ImGui::End();
			return;
		}

		ImGui::Text(("ID del objeto: " + std::to_string(selected_object_ptr->id)).c_str());
		ImGui::Text(("Nombre del objeto: " + selected_object_ptr->name).c_str());
		ImGui::Text(("Componentes: " + std::to_string(selected_object_ptr->get_components_size())).c_str());

		ImGui::Separator();
		ImGui::Spacing();

		// PRUEBAAA //
		// USAR LA GLOBAL PARA VER LOS COMPONENTES CREADOS //

		if (ImGui::BeginCombo("##components", components_to_add[component_selected_to_add].name))
		{
			// iterar a traves de los componentes y agarrar sus nombres
			for (int i = 0; i < components_to_add.size(); ++i)
			{
				if (ImGui::Selectable(components_to_add[i].name, component_selected_to_add == i))
				{
					component_selected_to_add = i;
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("add"))
		{
			// añadir al set de componentes del gameobject seleccionado
			selected_object_ptr->add_component(components_to_add[component_selected_to_add].return_comp());
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// cargamos la interfaz de cada componente del GameObject seleccionado:
		for (auto& c : selected_object_ptr->get_components())
		{
			c->draw_header();
			if (selected_object_ptr->get_components_size() > 1)
			{
				ImGui::Spacing();
			}
		}
	}
	ImGui::End();
}