#pragma once
#include <unordered_map>
#include <string>

void ThemeManager_add_theme(std::string name, void (*func)());

std::unordered_map<std::string, void (*)()>& ThemeManager_get_themes();

std::pair<std::string, void (*)()>& ThemeManager_get_selected_theme();