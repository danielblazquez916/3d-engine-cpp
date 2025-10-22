#include "Transform.hpp"
#include <IMGUI/imgui.h>
#include "GameObject.hpp"
#include <IMGUI/ImGuizmo.h>
#include "Camera.hpp"
#include <iostream>

extern GameObject* selected_object_ptr;

ImGuizmo::OPERATION actual_op = ImGuizmo::ROTATE;

Transform::Transform() : scale(1.f), rotate(0.f), pos(0.f), model(1.f), Component("Transform") {};

void Transform::update(float& elapsed_time, std::vector<Instruction>& instrucciones)
{
	// simplemente cuadrar las instrucciones, y ejecutarlas cada frame
	for (auto& ins : instrucciones)
	{
		std::vector<std::string>& path = ins.path;
		// path = prop + coord
		// prop -> "rotation", "position", "scale"
		// coord -> x, y, z

		glm::vec3* prop = nullptr;
		float* coordinate = nullptr;

		if (path[0] == "rotation") prop = &rotate;
		else if (path[0] == "position") prop = &pos;
		else if (path[0] == "scale") prop = &scale;
		if (prop == nullptr) continue;


		if (path[1] == "x") coordinate = &prop->x;
		else if (path[1] == "y") coordinate = &prop->y;
		else if (path[1] == "z") coordinate = &prop->z;
		if (coordinate == nullptr) continue;

		// finalmente, lo aplicamos:
		float res = ins.operador(*coordinate, (ins.val * elapsed_time));
		*coordinate = res;

		apply_model();
	}
}

void Transform::apply_model()
{
	glm::mat4 S = glm::scale(glm::mat4(1.f), scale);

	// hacemos lo mismo que antes pero al reves
	glm::quat quatRotation = glm::quat(glm::radians(rotate));
	glm::mat4 R = glm::toMat4(quatRotation);

	glm::mat4 T = glm::translate(glm::mat4(1.f), pos);

	model = T * R * S;
}

void Transform::extract_transformations()
{
	// ultima columna de la matriz:
	pos = glm::vec3(model[3]);

	// la escala esta en la longitud de los vectores columna:
	scale.x = glm::length(glm::vec3(model[0]));
	scale.y = glm::length(glm::vec3(model[1]));
	scale.z = glm::length(glm::vec3(model[2]));

	glm::mat4 R = model;
	R[0] /= scale.x;
	R[1] /= scale.y;
	R[2] /= scale.z;

	// lo paso a quaternion para despues pasarlo a angulos de euler
	// y obtener los grados en cada eje:
	glm::quat q = glm::quat_cast(R);
	glm::vec3 euler = glm::degrees(glm::eulerAngles(q)); // esta en radianes por eso lo convierto

	rotate = euler;
}

void Transform::draw_header()
{
	// creamos un nodo para el Transform y otro para el Mesh (obligatorios):
	if (ImGui::CollapsingHeader("Transform"))
	{
		Camera* cam = (Camera*)selected_object_ptr->get_component_by_name("Camera");

		const char* axis[3] = { "X", "Y", "Z" };
		ImGui::Spacing();

		/*
			************* POSICIÓN *************
		*/
		ImGui::PushID("Position");

		ImGui::Text("Position");
		ImGui::Separator();
		ImGui::Spacing();

		for (int i = 0; i < 3; ++i)
		{
			ImGui::SameLine();
			ImGui::Text(axis[i]);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);

			if (ImGui::DragFloat((std::string("##") + axis[i]).c_str(), &pos[i], .1f))
			{
				actual_op = ImGuizmo::TRANSLATE;
				apply_model();
				if (cam != nullptr) cam->update_view_matrix();
			}
		}

		ImGui::PopID();


		/*
			************* ROTACIÓN *************
		*/
		ImGui::PushID("Rotation");

		ImGui::Text("Rotation");
		ImGui::Separator();
		ImGui::Spacing();

		for (int i = 0; i < 3; ++i)
		{
			ImGui::SameLine();
			ImGui::Text(axis[i]);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			if (ImGui::DragFloat((std::string("##") + axis[i]).c_str(), &rotate[i], 1.f))
			{
				actual_op = ImGuizmo::ROTATE;
				apply_model();
				if (cam != nullptr) cam->update_view_matrix();
			}
		}

		ImGui::PopID();


		/*
			************* ESCALA *************
		*/
		ImGui::PushID("Scale");

		ImGui::Text("Scale");
		ImGui::Separator();
		ImGui::Spacing();

		for (int i = 0; i < 3; ++i)
		{
			ImGui::SameLine();
			ImGui::Text(axis[i]);

			ImGui::SameLine();
			ImGui::SetNextItemWidth(60.0f);
			if (ImGui::DragFloat((std::string("##") + axis[i]).c_str(), &scale[i], .1f))
			{
				actual_op = ImGuizmo::SCALE;
				apply_model();
			}
		}

		ImGui::PopID();
		ImGui::Separator();
	}
}

nlohmann::json Transform::serialize()
{
	nlohmann::json trans_comp;

	// para la posicion...
	nlohmann::json position_ = nlohmann::json::array();
	position_.push_back(pos.x);
	position_.push_back(pos.y);
	position_.push_back(pos.z);
	trans_comp["position"] = position_;

	// para la rotacion...
	nlohmann::json rotation_ = nlohmann::json::array();
	rotation_.push_back(rotate.x);
	rotation_.push_back(rotate.y);
	rotation_.push_back(rotate.z);
	trans_comp["rotation"] = rotation_;

	// para la escala...
	nlohmann::json scale_ = nlohmann::json::array();
	scale_.push_back(scale.x);
	scale_.push_back(scale.y);
	scale_.push_back(scale.z);
	trans_comp["scale"] = scale_;

	return trans_comp;
}

void Transform::deserialize(const nlohmann::json& comp_nodo)
{
	// posicion
	auto& pos_node = comp_nodo["position"];
	pos = glm::vec3(pos_node[0].get<float>(), pos_node[1].get<float>(), pos_node[2].get<float>());

	// escala
	auto& scale_node = comp_nodo["scale"];
	scale = glm::vec3(scale_node[0].get<float>(), scale_node[1].get<float>(), scale_node[2].get<float>());

	// rotacion
	auto& rot_node = comp_nodo["rotation"];
	rotate = glm::vec3(rot_node[0].get<float>(), rot_node[1].get<float>(), rot_node[2].get<float>());

	// aplicamos modelo:
	apply_model();
	selected_object_ptr = nullptr;
}

