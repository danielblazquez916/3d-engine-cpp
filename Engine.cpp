#include "Engine.hpp"
#include "Constants.hpp"
#include <iostream>
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include <cstdint>
#include "Shader.hpp"
#include "VAOFactory.hpp"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <IMGUI/ImGuizmo.h>
#include "ThemeManager.hpp"
#include "SceneManager.hpp"
#include <filesystem>
#include "Camera.hpp"
#include "Script.hpp"
#include <future>

// VARIABLES GLOBALES //
std::vector<GameObject*> gameobjects;
ResourceManager* rm = nullptr;
DebugGUI* debug_window;

extern GameObject* selected_object_ptr;
extern Camera* active_camera;
extern ImGuizmo::OPERATION actual_op;

extern std::string actual_scene;

// variables solo visibles en fichero!!
static float last_x = WIDTH/2;
static float last_y = HEIGHT/2;
static float yaw = -90.f;
static float pitch = 0.f;
static glm::vec3 front = glm::vec3(0.f, 0.f, -1.f);

static unsigned int skybox_vao = 0;

static bool game_running = 0;
//

std::string skybox_name = "none";
unsigned int skybox_id = 0;
bool wireframe_mode = 0;

float ambiental[3] = { 0.3f, 0.25f, 0.1f };
float especular[3] = { 1.0f, 1.0f, 1.0f };
float difusa[3] = { 0.9f, 0.8f, 0.8f };


Engine::Engine() : 
	scene_window(nullptr), inspector_window(nullptr),
	ventana(nullptr), last_time(0.f), fps_counter(0), fps_visual(0), 
	vsync(1)
{
};

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (active_camera != nullptr || game_running) return;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
	{
		last_x = xpos;
		last_y = ypos;

		return;
	}

	selected_object_ptr = nullptr;

	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;
	last_x = xpos;
	last_y = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// std::cout << yaw << "--" << pitch << '\n';

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
}

void Engine::draw_main_menu(Shader* sh)
{
	bool open_popup = 0;

	std::vector<std::string> scenes = { };
	static std::string scene_selected;

	// menu superior
	if (ImGui::BeginMainMenuBar())
	{
		if (!game_running && ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
				// encontrar nombres repetidos:
				std::string name = "default";
				bool duplicate_found = 1;
				while (duplicate_found)
				{
					duplicate_found = 0;
					for (const auto& scene : std::filesystem::directory_iterator("./scenes/"))
					{
						if (name == scene.path().stem().string())
						{
							duplicate_found = 1;

							// en el caso de que encuentre uno igual:
							std::string number;

							for (char& c : name)
								if (std::isdigit(c)) number += c;

							if (number.empty())
							{
								name += '1';
								break;
							}
							number = std::to_string(std::stoi(number) + 1);
							name = "default" + number;
						}
					}
				}

				if(!actual_scene.empty()) SceneManager_save_scene(actual_scene.c_str());

				// cargar la escena nueva:
				actual_scene = name + ".sc";
				SceneManager_create_empty_scene(actual_scene.c_str());
				if(SceneManager_load_scene(actual_scene.c_str()))
					debug_window->LOG(("[Engine] ~ Escena \"" + actual_scene + "\" creada con exito!").c_str(), nullptr);
			}

			bool selected_scene_reset = 1;
			if (ImGui::BeginMenu("Load"))
			{
				// cargar las escenas:
				for (const auto& scene : std::filesystem::directory_iterator("./scenes/"))
				{
					if (scene.path().filename().string() == scene_selected) selected_scene_reset = 0;

					if (scene.path().extension().string() == ".sc")
						scenes.push_back(scene.path().filename().string());
				}

				if (selected_scene_reset) scene_selected = "";

				if (ImGui::BeginCombo("##openscene", scene_selected.c_str()))
				{
					for (int i = 0; i < scenes.size(); ++i)
					{
						if (ImGui::Selectable(scenes[i].c_str(), scene_selected == scenes[i]))
						{
							scene_selected = scenes[i];
						}
					}

					ImGui::EndCombo();
				}

				ImGui::BeginDisabled(scene_selected.empty());
				if (ImGui::Button("open"))
				{
					ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]);
					debug_window->LOG("[Engine] ~ Cargando escena...", color);

					if(SceneManager_load_scene(scene_selected.c_str()))
						debug_window->LOG("[Engine] ~ Escena cargada con exito!", nullptr);
				}
				ImGui::EndDisabled();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Save as"))
			{
				ImGui::Text("Nombre de escena: ");

				char buffer[20] = "";
				if (ImGui::InputText("##save_as", buffer, 20, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]);
					debug_window->LOG("[Engine] ~ Guardando escena...", color);

					// cambiar el nombre del archivo ya guardado:
					if (!actual_scene.empty())
					{
						for (auto& scene : std::filesystem::directory_iterator("./scenes/"))
						{
							if (actual_scene == scene.path().filename().string())
							{
								std::filesystem::path new_name = std::string("./scenes/") + buffer + std::string(".sc");
								std::filesystem::rename(scene.path(), new_name);
								break;
							}
						}
					}

					actual_scene = buffer + std::string(".sc");
					if(SceneManager_save_scene(actual_scene.c_str()))
						debug_window->LOG("[Engine] ~ Escena guardada con exito!!", nullptr);
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Save"))
			{
				ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]);
				if (!actual_scene.empty())
				{
					// guardar la escena actual
					debug_window->LOG("[Engine] ~ Guardando...", color);

					if (SceneManager_save_scene(actual_scene.c_str()))
						debug_window->LOG(("[Engine] ~ Escena \"" + actual_scene + "\" guardada con exito!!").c_str(), nullptr);
				}
				else
				{
					debug_window->LOG("[Engine] ~ Usa \"Save As\" o \"New\" para nombrar la escena.", color);
				}
			}

			ImGui::EndMenu();
		}

		if (!game_running && ImGui::BeginMenu("Settings"))
		{
			// activar el modo wireframe
			ImGui::Text("Modo wireframe: ");
			ImGui::SameLine();
			ImGui::Checkbox("##wireframe_mode", &wireframe_mode);

			(wireframe_mode) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				
			ImGui::Spacing();

			// cambiar el skybox
			std::vector<std::string> cubemaps = {"none"};

			for (const auto& cubemap : std::filesystem::directory_iterator("./cubemaps/"))
			{
				if (cubemap.is_directory()) cubemaps.push_back(cubemap.path().filename().string());
			}

			ImGui::Text("Cambiar skybox: ");
			if (ImGui::BeginCombo("##cubemaps", skybox_name.c_str())) // <---
			{
				for (int i = 0; i < cubemaps.size(); ++i)
				{
					if (ImGui::Selectable(cubemaps[i].c_str(), skybox_name == cubemaps[i]))
					{
						skybox_name = cubemaps[i];

						if (skybox_name == "none")
						{
							skybox_id = 0;
							break;
						}

						// cargar el skybox seleccionado:
						skybox_id = (uintptr_t)rm->load_resource(RES_TYPE::CUBEMAP, "./cubemaps/"+cubemaps[i]+"/");
						if (!skybox_id) break;

						skybox_name = cubemaps[i];
					}
				}

				ImGui::EndCombo();
			}

			ImGui::Spacing();

			// cambiar el color de la luz direccional (o incluso otros parametros)
			if (ImGui::BeginMenu("Luz direccional"))
			{
				ImGui::Text("Color de luz ambiental: ");
				if (ImGui::ColorEdit3("##ambiental", ambiental))
					sh->set_vec3_uniform("light.ambient", glm::vec3(ambiental[0], ambiental[1], ambiental[2]));

				ImGui::Spacing();

				ImGui::Text("Color de luz especular: ");
				if(ImGui::ColorEdit3("##especular", especular))
					sh->set_vec3_uniform("light.specular", glm::vec3(especular[0], especular[1], especular[2]));

				ImGui::Spacing();

				ImGui::Text("Color de luz difusa: ");
				if(ImGui::ColorEdit3("##difusa", difusa))
					sh->set_vec3_uniform("light.diffuse", glm::vec3(difusa[0], difusa[1], difusa[2]));

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		auto& themes = ThemeManager_get_themes();
		auto& selected_theme = ThemeManager_get_selected_theme();

		// seccion paa cambiar el tema de colores
		if (!game_running && ImGui::BeginMenu("Themes"))
		{
			ImGui::Text("Change theme: ");
			if (ImGui::BeginCombo("##theme", selected_theme.first.c_str())) // <---
			{
				for (auto& theme : themes)
				{
					if (ImGui::Selectable(theme.first.c_str(), selected_theme == theme))
					{
						selected_theme = theme;
						theme.second();
					}
				}

				ImGui::EndCombo();
			}
			ImGui::Spacing();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Juego"))
		{
			ImGui::BeginDisabled(actual_scene.empty());
			if (ImGui::MenuItem(game_running ? "Parar" : "Iniciar"))
			{
				if (game_running == 0)
				{
					ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]);
					debug_window->LOG("[SceneManager] ~ Iniciando el juego...", color);
					game_running = 1;

					// tokenizamos las instrucciones de los objetos que tengan script:
					static std::unordered_map<std::string, std::string> parse_errors;
					for (auto& obj : gameobjects)
					{
						Script* scr = (Script*)obj->get_component_by_name("Script");
						if (scr == nullptr) continue;
						scr->tokenize();
						if (!scr->error.empty()) parse_errors.insert({ scr->script_selected, scr->error });
					}

					for (auto& error : parse_errors)
					{
						ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
						debug_window->LOG(error.second.c_str(), color);
						game_running = 0;
					}

					parse_errors.clear();

					if (game_running)
					{
						// guardamos escena:
						SceneManager_save_scene(actual_scene.c_str());

						if (active_camera == nullptr)
						{
							ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
							debug_window->LOG("[Engine] ~ No hay ninguna camara activa, por eso no ves nada!", color);
						}

						// liberamos las ventanas:
						// delete debug_window;
						delete inspector_window;
						delete scene_window;

						// debug_window = nullptr;
						inspector_window = nullptr;
						scene_window = nullptr;

						debug_window->LOG("[SceneManager] ~ Juego iniciado con exito!", nullptr);
					}
				}
				else
				{
					game_running = 0;
					// cargamos la escena con los datos guardados antes de empezar:
					SceneManager_load_scene(actual_scene.c_str());
					
					// creamos las ventanas de nuevo:
					scene_window = new SceneGUI();
					inspector_window = new InspectorGUI();
					//debug_window = new DebugGUI();
				}
			}

			ImGui::EndDisabled();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Engine::run()
{
	// camara del editor:
	glm::mat4 view(1.f);

	glm::vec3 editor_camera_pos = glm::vec3(0.0f, 0.0f, 7.0f);
	glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
	float camera_speed = 5.f;

	view = glm::lookAt(editor_camera_pos, editor_camera_pos + front, up_vector);

	glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)WIDTH/(float)HEIGHT, 0.1f, 1000.f);

	////////////////////////////////////////////////////////////////////////////////////////////////

	// cargamos el tema predeterminado del motor:
	ThemeManager_get_selected_theme().second();

	// cargamos el shader para la escena:
	Shader* sh = (Shader*) rm->load_resource(RES_TYPE::SHADER, "./shaders/prueba");
	if (sh == nullptr)
	{
		ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]); // mas anaranjado para warnings
		debug_window->LOG("[Engine] ~ Si el Shader no funciona correctamente, lo mejor es que reinicie el motor o lo reinstale. No lo siga usando.", color);
	}

	// shader y vao para el skybox:
	skybox_vao = create_skybox_vao();
	Shader* sh_cm = (Shader*)rm->load_resource(RES_TYPE::SHADER, "./shaders/cubemap");
	if (sh_cm == nullptr)
	{
		ImVec4* color = new ImVec4(WARNING[0], WARNING[1], WARNING[2], WARNING[3]); // mas anaranjado para warnings
		debug_window->LOG("[Engine] ~ Si el Shader no funciona correctamente, lo mejor es que reinicie el motor o lo reinstale. No lo siga usando.", color);
	}

	// cargar informacion para la luz direccional al shader:
	sh->set_vec3_uniform("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	
	sh->set_vec3_uniform("light.ambient", glm::vec3(0.3f, 0.25f, 0.1f));
	sh->set_vec3_uniform("light.diffuse", glm::vec3(0.9f, 0.8f, 0.8f));
	sh->set_vec3_uniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// activar/desactivarlo
	glfwSwapInterval(vsync);

	last_time = glfwGetTime();
	float last_time_delta = glfwGetTime();

	while (glfwWindowShouldClose(ventana) == 0)
	{
		// contar deltatime (para la ejecucion del juego)
		float current_time = glfwGetTime();
		float delta_time = current_time - last_time_delta;
		last_time_delta = current_time;

		// contar los fps para depuracion:
		float diff = current_time - last_time;
		++fps_counter;
		if (diff >= 1.f) // 1 segundo
		{
			fps_visual = fps_counter;
			fps_counter = 0;
			last_time = glfwGetTime();
		}

		// 1º) procesar la cola de eventos:
		glfwPollEvents();

		// 2º) inputs:
		if (!ImGui::GetIO().WantCaptureKeyboard && !game_running && active_camera == nullptr)
		{
			if (glfwGetKey(ventana, GLFW_KEY_W) == GLFW_PRESS)
				editor_camera_pos += (camera_speed * delta_time) * front;
			if (glfwGetKey(ventana, GLFW_KEY_S) == GLFW_PRESS)
				editor_camera_pos -= (camera_speed * delta_time) * front;
			if (glfwGetKey(ventana, GLFW_KEY_A) == GLFW_PRESS)
				editor_camera_pos -= glm::normalize(glm::cross(front, up_vector)) * (camera_speed * delta_time);
			if (glfwGetKey(ventana, GLFW_KEY_D) == GLFW_PRESS)
				editor_camera_pos += glm::normalize(glm::cross(front, up_vector)) * (camera_speed * delta_time);
		}
		
		glfwSetCursorPosCallback(ventana, [](GLFWwindow* window, double xpos, double ypos)
		{
			ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

			if (!ImGui::GetIO().WantCaptureMouse)
			{
				mouse_callback(window, xpos, ypos);
			}
		});

		// actualizamos la matriz de vista:
		view = glm::lookAt(editor_camera_pos, editor_camera_pos + front, up_vector);
		if (active_camera != nullptr) active_camera->update_view_matrix();

		if (game_running)
		{
			// update:
			for (auto& obj : gameobjects)
			{
				obj->update(delta_time);
			}
		}

		// 3º) limpiar buffers:
		glClearColor(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2], BG_COLOR[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Iniciar frames de ImGui:
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!game_running)
		{
			// Iniciar frames de ImGuizmo:
			ImGuizmo::BeginFrame();
			ImGuizmo::SetOrthographic(0);
			ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

			// al seleccionar un objeto, que los gizmos funcionen:
			if (selected_object_ptr != nullptr)
			{
				Transform* trans = (Transform*)selected_object_ptr->get_component_by_name("Transform");

				ImGuizmo::SetRect(0, 0, WIDTH, HEIGHT);
				ImGuizmo::Manipulate(
					glm::value_ptr(active_camera != nullptr ? active_camera->view_matrix : view),
					glm::value_ptr(active_camera != nullptr ? active_camera->projection_matrix : proj),
					actual_op,
					ImGuizmo::LOCAL,
					glm::value_ptr(trans->model));

				if (ImGuizmo::IsUsing())
				{
					trans->extract_transformations();
				}
			}
		}

		if (active_camera == nullptr)
			sh->set_vec3_uniform("view_pos", editor_camera_pos);
		else
		{
			Transform* tr_act_cam = (Transform*)active_camera->owner->get_component_by_name("Transform");
			sh->set_vec3_uniform("view_pos", tr_act_cam->pos);
		}

		// 5º) renderizar mis objetos:

		if (skybox_id > 0)
		{
			glDepthFunc(GL_LEQUAL);
			sh_cm->use();

			glm::mat4 vp = glm::mat4(1.f);

			vp = (active_camera == nullptr) ? (proj * glm::mat4(glm::mat3(view))) : (active_camera->projection_matrix * glm::mat4(glm::mat3(active_camera->view_matrix)));
			sh_cm->set_mat4_uniform("vp", vp);

			glBindVertexArray(skybox_vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_id);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);
		}

		sh->use();
		for (const GameObject* obj : gameobjects)
		{
			Transform* trans = (Transform*)obj->get_component_by_name("Transform");
			Model* model = (Model*)obj->get_component_by_name("Model");

			glm::mat4 mvp = glm::mat4(1.f);

			// crear la matriz de computo (MVP):
			mvp = (active_camera == nullptr) ? (proj * view * trans->model) : (active_camera->projection_matrix * active_camera->view_matrix * trans->model);
			sh->set_mat4_uniform("mvp", mvp);

			// cargar el modelo tambien para calculos de iluminacion
			sh->set_mat4_uniform("model", trans->model);

			if (game_running && active_camera == nullptr) continue;

			model->draw_model(sh);
		}
		
		draw_main_menu(sh);

		// renderizar las ventanas del editor
		if(scene_window != nullptr) scene_window->render(fps_visual, vsync);
		if (inspector_window != nullptr) inspector_window->render();
		if (debug_window != nullptr) debug_window->render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 6º) swap:
		glfwSwapBuffers(ventana);
	}
}

void Engine::finish()
{
	// destruimos el contexto asociado a imgui:
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	// NO HACE FALTA, pero por si tengo varias ventanas:
	if (ventana != nullptr)
	{
		glfwDestroyWindow(ventana);
		ventana = nullptr;
	}

	glfwTerminate();
}

bool Engine::init()
{
	// Inicializar recursos de GLFW y preferencias a la hora de crear la ventana:
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// crear la ventana que ASOCIARÁ el contexto de OpenGL (uno por hilo):
	ventana = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
	if (ventana == 0)
	{
		std::cerr << "[System] ~ No se ha podido inicializar la ventana del motor.\n";
		glfwTerminate();
		return 0; // 0 = false
	}
	glfwMakeContextCurrent(ventana);

	// cargar glad:
	GLADloadproc toload = (GLADloadproc)glfwGetProcAddress;
	if (gladLoadGLLoader(toload) == 0)
	{
		std::cerr << "[System] ~ la biblioteca GLAD no funciona, el motor no se puede iniciar.\n";
		glfwTerminate();
		return 0;
	}

	// cargar ImGui:
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); // solo para eventos creados por mi

	ImFont* f = io.Fonts->AddFontFromFileTTF("./Fonts/Roboto-Regular.ttf", 16.0f);

	ImGui_ImplGlfw_InitForOpenGL(ventana, 1);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Inicializar ventanas:
	scene_window = new SceneGUI();
	inspector_window = new InspectorGUI();
	debug_window = new DebugGUI();

	if (f == nullptr)
	{
		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG("[Engine] ~ Ha habido un error en la importacion de la fuente.", color);
	}

	//  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST); // importante para que sea de utilidad el z-buffer

	// CARGAMOS RESOURCE MANAGER (una vez inicializado todo):
	rm = new ResourceManager();

	return 1;
}