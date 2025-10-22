#pragma once

// funciones para guardar y cargar escenas basicamente
bool SceneManager_save_scene(const char* filename);

bool SceneManager_load_scene(const char* filename);

bool SceneManager_create_empty_scene(const char* filename);