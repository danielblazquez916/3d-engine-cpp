#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include "Component.hpp"


class Transform : public Component
{
public:
	glm::mat4 model;

	glm::vec3 scale;
	glm::vec3 rotate; // cada valor es el angulo en GRADOS (luego se convierte en radianes) en cada coordenada;
	glm::vec3 pos;

	Transform();
	~Transform() = default;

	void extract_transformations();

	// aplica las 3 transformaciones al modelo del objeto:
	void apply_model();

	// dibujar la interfaz del header del Transform:
	void draw_header() override;
	void update(float& elapsed_time, std::vector<Instruction>& instrucciones) override;

	// serializar / deserializar en JSON???
	nlohmann::json serialize() override;
	void deserialize(const nlohmann::json& comp_nodo) override;
};