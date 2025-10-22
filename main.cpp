#include "Engine.hpp"
#include <iostream>
#include "Constants.hpp"

extern DebugGUI* debug_window;

int main()
{
	Engine serlliber_engine;

	if (serlliber_engine.init() == 0) return -1;

	debug_window->LOG("[Engine] ~ El motor se ha inicializado correctamente.", nullptr);

	ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]);
	debug_window->LOG("[SceneManager] ~ Crea una escena antes de ejecutar el juego.", color);

	serlliber_engine.run();

	serlliber_engine.finish();
}