#pragma once
#include <vector>
#include <string>
#include <IMGUI/imgui.h>

struct MY_LOG
{
	std::string text;
	ImVec4* color;
};

class DebugGUI
{
public:
	// Simple y directo, para colocar mensajes de error, información...

	DebugGUI() = default;
	~DebugGUI() = default;

	void render();
	void LOG(const char* text, ImVec4* color); // en un futuro lo cambiaré por varargs
	
private:
	std::vector<MY_LOG> logs;
};