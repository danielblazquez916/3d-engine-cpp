#include "Camera.hpp"
#include <GLM/gtc/matrix_transform.hpp>
#include "Constants.hpp"
#include "DebugGUI.hpp"
#include <iostream>
#include "GameObject.hpp"

extern GameObject* selected_object_ptr;

// camara activa para el juego:
Camera* active_camera = nullptr;

Camera::Camera(GameObject* owner) : Component("Camera"), view_matrix(1.f), projection_matrix(1.f),
FOV(45.f), near(2.f), far(1000.f), owner(owner)
{
	update_view_matrix();
	update_projection_matrix();
};

void Camera::set_perspective(float FOV, float near_p, float far_p)
{
	// actualizamos los valores:
	this->FOV = FOV;
	near = near_p;
	far = far_p;
}

void Camera::update_projection_matrix()
{
	projection_matrix = glm::perspective(glm::radians(FOV), (float)WIDTH/(float)HEIGHT, near, far);
}

void Camera::update_view_matrix()
{
	// transladar la camara a la posicion del objeto:
	//if (owner == nullptr) return; // no deberia de ocurrir eso;
	Transform* tr = (Transform*) owner->get_component_by_name("Transform");

	glm::vec3& obj_pos = tr->pos;
	/*
	glm::vec3& obj_rot = tr->rotate;
	
	glm::vec3 forward;
	forward.x = cos(glm::radians(obj_rot.y)) * cos(glm::radians(obj_rot.x));
	forward.y = sin(glm::radians(obj_rot.x));
	forward.z = sin(glm::radians(obj_rot.y)) * cos(glm::radians(obj_rot.x));
	*/

	glm::vec3 forward = -glm::normalize(glm::vec3(tr->model[2]));

	view_matrix = glm::lookAt(obj_pos, obj_pos+forward, glm::vec3(0.f, 1.f, 0.f));

	//tr->apply_model();
}

void Camera::draw_header()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		// editar el FOV:
		if (ImGui::InputFloat("FOV", &FOV, 1.f, 5.f))
		{
			update_projection_matrix();
		}

		ImGui::Spacing();

		// editar el near plane:
		if (ImGui::InputFloat("Near Plane", &near, 0.1f, 1.f))
		{
			update_projection_matrix();
		}

		ImGui::Spacing();

		// editar el far plane:
		if (ImGui::InputFloat("Far Plane", &far, 5.f, 100.f))
		{
			update_projection_matrix();
		}

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button(active_camera != this ? "Activate camera" : "Deactivate camera"))
		{
			if (active_camera != this) active_camera = this;
			else active_camera = nullptr;
		}

		ImGui::Separator();

		if(ImGui::Button("remove"))
		{
			selected_object_ptr->remove_component("Camera");
			active_camera = nullptr;
		}

		ImGui::Separator();
	}
}

nlohmann::json Camera::serialize()
{
	nlohmann::json camera_node;

	camera_node["fov"] = FOV;
	camera_node["near_plane"] = near;
	camera_node["far_plane"] = far;
	camera_node["is_active"] = active_camera == this;

	return camera_node;
}

void Camera::deserialize(const nlohmann::json& comp_nodo)
{
	FOV = comp_nodo["fov"].get<float>();
	near = comp_nodo["near_plane"].get<float>();
	far = comp_nodo["far_plane"].get<float>();
	active_camera = comp_nodo["is_active"] ? this : nullptr;

	update_projection_matrix();
}