#include "GameObject.hpp"
#include "Transform.hpp"
#include <iostream>
#include "DebugGUI.hpp"
#include "Constants.hpp"
#include "Script.hpp"
#include "Camera.hpp"

extern DebugGUI* debug_window;

GameObject::GameObject(int id, const std::string& name) : id(id), name(name) 
{
	// de forma predeterminada estará el Transform y el Mesh (que no se podrán eliminar):
	add_component(new Transform());
	add_component(new Model("default"));
};

GameObject::~GameObject()
{
	std::cout << "eliminado\n";
	for (auto& c : componentes)
	{
		delete c;
		c = nullptr;
	}

	componentes.clear();
}

Component* GameObject::get_component_by_name(const std::string& name) const
{
	for (auto& c : componentes)
	{
		if (c->name == name)
		{
			return c;
		}
	}

	return nullptr;
}

void GameObject::update(float& elapsed_time)
{
	// obtenemos el script component (si hay):
	Script* scr = (Script*) get_component_by_name("Script");
	if (scr == nullptr) return;

	std::unordered_map<std::string, std::vector<Instruction>>* instrs = nullptr;

	// extraemos las intrucciones de la funcion "update"
	for (Function& func : scr->funciones)
	{
		if (func.name == "update")
		{
			instrs = &func.instructions;
		}
	}

	if (instrs == nullptr) return;

	// aplicamos las instrucciones correspondientes a cada componente
	for (auto& c : componentes)
	{
		std::vector<Instruction>* ins = nullptr;

		auto it = instrs->find(c->name);
		if (it != instrs->end())
		{
			ins = &it->second;
		}

		if (ins == nullptr) continue;

		// finalmente, si hay instrucciones, actualizamos:
		c->update(elapsed_time, *ins);
	}
}

void GameObject::remove_component(const std::string& name)
{
	for (auto it = componentes.begin(); it != componentes.end(); )
	{
		if ((*it)->name == name)
		{
			delete *it; // importante liberar la memoria!!
			componentes.erase(it);
			return;
		}

		++it;
	}
}

std::vector<Component*>& GameObject::get_components()
{
	return componentes; // uso el original
}

void GameObject::add_component(Component* c)
{
	for (auto& comp : componentes)
	{
		if (comp->name == c->name)
		{
			ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
			debug_window->LOG(("[Engine] ~ El componente " + comp->name + " ya se ha colocado en \"" + this->name + "\", prueba con otro.").c_str(), color);

			return;
		}
	}

	if (c->name == "Camera")
	{
		Camera* cam = (Camera*) c;
		cam->owner = this;
	}

	componentes.push_back(c);
}

int GameObject::get_components_size() const
{
	return componentes.size();
}

nlohmann::json GameObject::serialize()
{
	nlohmann::json obj;
	obj["id"] = id;
	obj["name"] = name;

	nlohmann::json comp;
	for (const auto& c : componentes)
	{
		comp[c->name] = c->serialize();
	}

	obj["components"] = comp;

	return obj;
}

void GameObject::deserialize(const nlohmann::json& obj_nodo)
{
	// iterar por cada componente:
	nlohmann::json comps = obj_nodo["components"];
	for (auto it = comps.begin(); it != comps.end(); ++it)
	{
		// tanto mesh como transform vienen de forma predeterminada:
		if (it.key() == "Transform")
		{
			Transform* tr = (Transform*) get_component_by_name("Transform");
			tr->deserialize(it.value());
		}
		
		if (it.key() == "Model")
		{
			Model* model = (Model*) get_component_by_name("Model");
			model->deserialize(it.value());
		}

		// extras:
		if (it.key() == "Script")
		{
			Script* sc = new Script();
			sc->deserialize(it.value());
			componentes.push_back(sc);
		}

		if (it.key() == "Camera") 
		{
			Camera* cam = new Camera(this);
			cam->deserialize(it.value());
			componentes.push_back(cam);
		}
	}
}