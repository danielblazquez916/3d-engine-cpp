#include "Script.hpp"
#include <IMGUI/imgui.h>
#include <vector>
#include <string>
#include <filesystem>
#include "GameObject.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

extern GameObject* selected_object_ptr;

Script::Script() : script_selected("none"), script_enabled(1), Component("Script"), error("") {};


void Script::tokenize()
{
	funciones.clear();
	error = "";

	// leer el fichero del script y guardar las instrucciones:
	std::ifstream ifs("./scripts/"+script_selected);
	std::string line;
	int line_counter = 0;

	Function block;

	while (std::getline(ifs, line))
	{
		++line_counter;
		// (para quitar espacios innecesarios, tu sabeee)
		line.erase(line.find_last_not_of(" \t\n\r") + 1);
		line.erase(0, line.find_first_not_of(" \t\n\r"));

		if (line.empty()) continue;

		if (line == "start" || line == "update")
		{
			if (!block.name.empty())
			{
				// EXCEPCION: Se ha iniciado otro bloque sin terminar el anterior.
				error += "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") Se ha iniciado otro bloque sin terminar el anterior.";
				return;
			}
			block.name = line;
			continue;
		}

		if (line == "end")
		{
			if (block.name.empty())
			{
				// EXCEPCION: Se ha terminado un bloque sin haber iniciado uno.
				error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") Se ha terminado un bloque sin haber iniciado uno.";
				return;
			}
			
			// mandamos el bloque:
			funciones.push_back(block);

			// limpiamos...
			block.instructions.clear();
			block.name = "";

			continue;
		}

		// si llega aqui sin haber iniciado una funcion: maal!
		if (block.name.empty())
		{
			error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") No se pueden ejecutar instrucciones si no hay al menos un bloque iniciado.";
			return;
		}

		// tokenizamos cada parte de la orden:
		std::vector<std::string> tokens;

		{
			std::stringstream ss(line);
			std::string token;
			while (ss >> token)
			{
				tokens.push_back(token);
			}
		}

		if (tokens.size() < 3)
		{
			error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") Faltan tokens por reconocer.";
			return;
		}

		// el primer token va separado por puntos (ej: "transform.pos.x" o "transform.rotate.x")
		std::vector<std::string> component_mod;

		{
			std::stringstream ss(tokens[0]);
			std::string token;

			while (std::getline(ss, token, '.'))
			{
				component_mod.push_back(token);
			}
		}

		if (component_mod.size() < 3)
		{
			error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") No se reconocen bien los componentes del token.";
			return;
		}

		// detectar si es "transform":
		if (!component_mod.empty() && component_mod[0] == "transform")
		{
			std::string prop = component_mod[1];
			std::string coord = component_mod[2];

			Instruction ins;

			// operadores
			if (tokens[1] == "add")
				ins.operador = [](float a, float b) { return a + b; };
			else if (tokens[1] == "sub")
				ins.operador = [](float a, float b) { return a - b; };
			else if (tokens[1] == "mul")
				ins.operador = [](float a, float b) { return a * b; };
			else if (tokens[1] == "div")
				ins.operador = [](float a, float b) { return a / b; };
			else
			{
				error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") No se reconoce el operador correctamente.";
				return;
			}

			ins.path = {prop, coord};

			try
			{
				ins.val = std::stof(tokens[2]);
			}
			catch (const std::invalid_argument& e)
			{
				error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") Parece ser que el valor que quieres añadir en cada tick no es valido. Comprueba que sea numerico.";
				return;
			}

			block.instructions["Transform"].push_back(ins);
		}
	}

	++line_counter;
	if (!block.name.empty())
	{
		error = "[SPL] [" + this->script_selected + "] ~ (Linea " + std::to_string(line_counter) + ") No se ha terminado el bloque con su respectivo \"end\".";
	}

	ifs.close();
}

void Script::draw_header()
{
	if (ImGui::CollapsingHeader("Script"))
	{
		ImGui::Text("Fichero seleccionado: ");
		ImGui::SameLine();

		std::vector<std::string> rutas_scripts = {"none"};

		// leer los ficheros .spl (Serlliber Property Language) de la carpeta /scripts
		for (const auto& file : std::filesystem::directory_iterator("./scripts/"))
		{
			// examino que tenga la extension correcta:
			if (file.path().extension().string() == ".spl")
			{
				rutas_scripts.push_back(file.path().filename().string());
			}
		}

		ImGui::SetNextItemWidth(140.f);
		if (ImGui::BeginCombo("##scripting", script_selected.c_str()))
		{
			for (int i = 0; i < rutas_scripts.size(); ++i)
			{
				if (ImGui::Selectable(rutas_scripts[i].c_str(), script_selected == rutas_scripts[i]))
				{
					script_selected = rutas_scripts[i];
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Text("Script activado: ");
		ImGui::SameLine();
		ImGui::Checkbox("##is_script_active", &script_enabled);

		ImGui::Spacing();

		if (ImGui::Button("remove"))
		{
			selected_object_ptr->remove_component("Script");
		}

		ImGui::Separator();
	}
}

nlohmann::json Script::serialize()
{
	nlohmann::json script_comp;

	script_comp["script_name"] = script_selected;
	script_comp["script_enabled"] = script_enabled;

	return script_comp;
}

void Script::deserialize(const nlohmann::json& comp_nodo)
{
	script_selected = comp_nodo["script_name"].get<std::string>();
	script_enabled = comp_nodo["script_enabled"].get<bool>();
}