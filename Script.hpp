#pragma once
#include "Component.hpp"
#include <unordered_map>
#include <vector>

class Script : public Component
{
public:
	std::string script_selected;
	bool script_enabled;

	std::vector<Function> funciones; // en principio solo leo "start" y "update"
	std::string error;

	Script();
	~Script() = default;

	void draw_header() override;

	void tokenize();

	void update(float& elapsed_time, std::vector<Instruction>& instrucciones) override
	{ // nada!!
	}

	nlohmann::json serialize() override;
	void deserialize(const nlohmann::json& comp_nodo) override;
};