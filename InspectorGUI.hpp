#pragma once
#include <IMGUI/ImGuizmo.h>

class InspectorGUI
{
public:
	// En esta, se mostrar� la informaci�n del objeto seleccionado. Esa informaci�n
	// son los componentes (Transform, Mesh...), y se podr�n modificar.

	InspectorGUI() = default;
	~InspectorGUI() = default;

	void render() const;

};