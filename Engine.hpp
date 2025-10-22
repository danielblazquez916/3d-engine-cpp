#pragma once
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <GLAD/include/glad.h>
#include <GLFW/glfw3.h>

#include "SceneGUI.hpp"
#include "InspectorGUI.hpp"
#include "DebugGUI.hpp"

#include "Shader.hpp"

//  _____           _ _ _ _                 _____            _            
// /  ___|         | | (_) |               |  ___|          (_)           
// \ `--.  ___ _ __| | |_| |__   ___ _ __  | |__ _ __   __ _ _ _ __   ___ 
//  `--. \/ _ \ '__| | | | '_ \ / _ \ '__| |  __| '_ \ / _` | | '_ \ / _ \
// /\__/ /  __/ |  | | | | |_) |  __/ |    | |__| | | | (_| | | | | |  __/
// \____/ \___|_|  |_|_|_|_.__/ \___|_|    \____/_| |_|\__, |_|_| |_|\___|
//                                                      __/ |             
//                                                     |___/              

class Engine
{
public:
	GLFWwindow* ventana;

	// ventanitas de ImGui:
	SceneGUI* scene_window;
	InspectorGUI* inspector_window;

	// para depuracion, fps y demás:
	int fps_counter;
	int fps_visual;
	double last_time;
	bool vsync; // limitar la tasa de frames segun los hercios de tu monitor basicamente

	Engine();

	bool init();
	void run();
	void draw_main_menu(Shader* sh);
	void finish();
};