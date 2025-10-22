#pragma once
#include <string>

class SceneGUI
{
public:
	// en esta, simplemente se mostrar� los diferentes objetos que
	// hay en la escena. Al pulsar, se guardar� un puntero con el objeto
	// seleccionado, y se usar� para mostrar su informaci�n en la ventana de
	// "Inspector".

	SceneGUI() = default;
	~SceneGUI() = default;

	void render(int& fps, bool& vsync) const;

	void add_gameobject(const std::string& obj_name) const;
};