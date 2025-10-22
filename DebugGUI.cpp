#include "DebugGUI.hpp"

void DebugGUI::LOG(const char* text, ImVec4* color)
{
	logs.push_back({text, color});
}

void DebugGUI::render()
{
	if (ImGui::Begin("Consola"))
	{
		if (ImGui::Button("Clear"))
		{
			// limpiar la consola
			for (auto it = logs.begin(); it != logs.end(); )
			{
				if(it->color != nullptr) delete it->color; // liberamos de la heap

				it = logs.erase(it);
			}
		}

		ImGui::Separator();

		if (ImGui::BeginChild("##logs", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
		{
			for (const MY_LOG& log : logs)
			{
				if (log.color == nullptr)
				{
					ImGui::TextWrapped(log.text.c_str());
					continue;
				}

				ImGui::PushStyleColor(ImGuiCol_Text, *log.color);
				ImGui::TextWrapped(log.text.c_str());
				ImGui::PopStyleColor();
			}

			if (!ImGui::GetIO().WantCaptureMouse)
				ImGui::SetScrollY(ImGui::GetScrollMaxY());
		}
		ImGui::EndChild();
	}
	ImGui::End();
}