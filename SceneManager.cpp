#include "SceneManager.hpp"
#include <JSON/json.hpp>
#include "GameObject.hpp"
#include <fstream>
#include "ResourceManager.hpp"
#include <GLAD/include/glad.h>
#include "DebugGUI.hpp"
#include "Constants.hpp"
#include "Camera.hpp"

extern std::vector<GameObject*> gameobjects;
extern int id_counting;
extern std::string skybox_name;
extern bool wireframe_mode;
extern unsigned int skybox_id;
extern GameObject* selected_object_ptr;
extern Camera* active_camera;

extern DebugGUI* debug_window;
extern ResourceManager* rm;

std::string actual_scene = "";

bool SceneManager_create_empty_scene(const char* filename)
{
	try
	{
		nlohmann::json scene;
		scene["id_counting"] = 1;
		scene["skybox_name"] = "none";
		scene["wireframe_mode"] = false;

		std::ofstream out(std::string("./scenes/") + filename);
		out << scene.dump(4);
		out.close();
	}
	catch (const nlohmann::json::exception& e)
	{
		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG("[SceneManager] ~ Ha habido un error al intentar guardar la escena.", color);
		return 0;
	}

	return 1;
}

bool SceneManager_save_scene(const char* filename)
{
	try
	{
		nlohmann::json scene;
		scene["id_counting"] = id_counting;
		scene["skybox_name"] = skybox_name;
		scene["wireframe_mode"] = wireframe_mode;

		for (const auto& obj : gameobjects)
		{
			scene["Objects"].push_back(obj->serialize());
		}
		std::ofstream out(std::string("./scenes/") + filename);
		out << scene.dump(4);
		out.close();
	}
	catch (const nlohmann::json::exception& e)
	{
		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG("[SceneManager] ~ Ha habido un error al intentar guardar la escena.", color);
		return 0;
	}

	return 1;
}

bool SceneManager_load_scene(const char* filename)
{
	try
	{
		std::ifstream file("./scenes/" + std::string(filename));

		nlohmann::json scene;
		file >> scene;

		// si existe...
		gameobjects.clear();
		selected_object_ptr = nullptr;
		active_camera = nullptr;

		id_counting = scene["id_counting"].get<int>();

		skybox_name = scene["skybox_name"].get<std::string>();
		if (skybox_name == "none") skybox_id = 0;
		else skybox_id = (uintptr_t)rm->load_resource(RES_TYPE::CUBEMAP, "./cubemaps/" + skybox_name + "/");

		wireframe_mode = scene["wireframe_mode"];
		(wireframe_mode) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		for (const auto& scene_obj : scene["Objects"])
		{
			GameObject* obj = new GameObject(scene_obj["id"].get<int>(), scene_obj["name"].get<std::string>());
			obj->deserialize(scene_obj);
			gameobjects.push_back(obj);
		}

		actual_scene = filename;
	}
	catch (const nlohmann::json::exception& e)
	{
		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG("[SceneManager] ~ Ha habido un error al intentar cargar la escena.", color);
		return 0;
	}

	return 1;
}