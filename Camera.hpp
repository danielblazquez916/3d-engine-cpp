#pragma once
#include "Component.hpp"
#include <GLM/glm.hpp>
#include "GameObject.hpp"

class Camera : public Component
{
public:
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;

	GameObject* owner;

	Camera(GameObject* owner);
	~Camera() = default;

	void update_view_matrix();
	void update_projection_matrix();

	void draw_header() override;

	void update(float& elapsed_time, std::vector<Instruction>& instrucciones) override
	{ // nada!!
	}

	void set_perspective(float FOV, float near_p, float far_p);

	nlohmann::json serialize() override;
	void deserialize(const nlohmann::json& comp_nodo) override;

private:
	float FOV;
	float near;
	float far;
};