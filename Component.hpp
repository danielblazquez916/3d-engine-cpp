#pragma once
#include <string>
#include <JSON/json.hpp>
#include <vector>

// PARA EL SCRIPT PARSER //
struct Instruction
{
	std::vector<std::string> path;
	float(*operador)(float, float) = nullptr;
	float val = 0;
};

struct Function
{
	std::string name;
	std::unordered_map<std::string, std::vector<Instruction>> instructions;
};
// PARA EL SCRIPT PARSER //

class Component
{
public:
	std::string name;

	Component(const std::string& name) : name(name) {};

	virtual ~Component() = default; // por si no se implementa, lo dejamos default

	virtual void draw_header() = 0;

	virtual void update(float& elapsed_time, std::vector<Instruction>& instrucciones) = 0; // debe ser OBLIGATORIO implementar

	virtual nlohmann::json serialize() = 0;
	virtual void deserialize(const nlohmann::json& comp_nodo) = 0;
};