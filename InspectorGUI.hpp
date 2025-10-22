#pragma once
#include <IMGUI/ImGuizmo.h>

class InspectorGUI
{
public:
	// En esta, se mostrará la información del objeto seleccionado. Esa información
	// son los componentes (Transform, Mesh...), y se podrán modificar.

	InspectorGUI() = default;
	~InspectorGUI() = default;

	void render() const;

};