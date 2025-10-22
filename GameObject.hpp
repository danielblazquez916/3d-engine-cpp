#pragma once
#include "Transform.hpp"
#include "Model.hpp"
#include <string>
#include "Component.hpp"
#include <vector>
#include <JSON/json.hpp>
   
// de momento es una estructura simple, luego se complicará

class GameObject
{
public:
	unsigned int id;
	std::string name;

	GameObject(int id, const std::string& name);
	~GameObject();

	Component* get_component_by_name(const std::string& name) const;

	void remove_component(const std::string& name);
	void add_component(Component* c);

	int get_components_size() const;

	std::vector<Component*>& get_components();
	void update(float& elapsed_time);

	// serializar y deserializar:
	nlohmann::json serialize();
	void deserialize(const nlohmann::json& obj_nodo);

private:
	std::vector<Component*> componentes;
};