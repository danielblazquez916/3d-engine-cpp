#pragma once
#include <string>

class SceneGUI
{
public:
	// en esta, simplemente se mostrará los diferentes objetos que
	// hay en la escena. Al pulsar, se guardará un puntero con el objeto
	// seleccionado, y se usará para mostrar su información en la ventana de
	// "Inspector".

	SceneGUI() = default;
	~SceneGUI() = default;

	void render(int& fps, bool& vsync) const;

	void add_gameobject(const std::string& obj_name) const;
};