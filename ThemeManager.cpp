#include "ThemeManager.hpp"
#include <IMGUI/imgui.h>
#include "DebugGUI.hpp"
#include "Constants.hpp"
#include <string>

extern DebugGUI* debug_window;

// temitas predeterminados:
static void DARK_RED_THEME()
{
	// reseteamos de fabrica:
	ImGuiStyle def; // no tiene nada por eso
	ImGui::GetStyle() = def;

	ImGui::StyleColorsDark();

	// DARK RED style by for40255 from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05490196123719215f, 0.05490196123719215f, 0.05490196123719215f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1176470592617989f, 0.1176470592617989f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.5647059082984924f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.5647059082984924f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9411764740943909f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.5647059082984924f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.0f, 0.0f, 0.0f, 0.501960813999176f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.0f, 0.0f, 0.8154506683349609f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8156862854957581f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.0f, 0.0f, 0.501960813999176f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.7450980544090271f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.6566523313522339f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.8039215803146362f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.501960813999176f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.6695278882980347f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9570815563201904f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1019607856869698f, 0.1137254908680916f, 0.1294117718935013f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2039215713739395f, 0.2078431397676468f, 0.2156862765550613f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 0.2000000029802322f);
	style.Colors[ImGuiCol_Tab] = ImVec4(1.0f, 0.0f, 0.0f, 0.4392156898975372f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 0.9725490212440491f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06666667014360428f, 0.06666667014360428f, 0.06666667014360428f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9490196108818054f, 0.3450980484485626f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4274509847164154f, 0.3607843220233917f, 0.3607843220233917f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.7124463319778442f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.196078434586525f, 0.196078434586525f, 0.196078434586525f, 0.6274510025978088f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.2705882489681244f, 0.3803921639919281f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1803921610116959f, 0.2274509817361832f, 0.2784313857555389f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}
static void WINDARK_THEME()
{
	// reseteamos de fabrica:
	ImGuiStyle def; // no tiene nada por eso
	ImGui::GetStyle() = def;

	ImGui::StyleColorsDark();

	// Windark style by DestroyerDarkNess from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 8.399999618530273f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 3.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 3.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 5.599999904632568f;
	style.ScrollbarRounding = 18.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 3.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}
static void DEFAULT_THEME()
{
	// reseteamos de fabrica:
	ImGuiStyle def; // no tiene nada por eso
	ImGui::GetStyle() = def;

	ImGui::StyleColorsDark();

	// serlliber style from ImThemes (el predeterminado de mi motor)
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 11.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 11.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 7.099999904632568f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.9807736277580261f, 0.9828326106071472f, 0.6369430422782898f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.7768239974975586f, 0.7114216685295105f, 0.2567186653614044f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(9.999999974752427e-07f, 6.964276622056786e-07f, 0.0f, 0.5862661123275757f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.06866955757141113f, 0.06866955757141113f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.8540772199630737f, 0.0f, 0.0f, 0.2532188892364502f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.6039110422134399f, 0.6051502227783203f, 0.3168597519397736f, 0.5400000214576721f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9828326106071472f, 0.0506180115044117f, 0.0506180115044117f, 0.4000000059604645f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.6652360558509827f, 0.5984293222427368f, 0.128479078412056f, 0.6700000166893005f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.003921568859368563f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5843137502670288f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.3948497772216797f, 0.1762419641017914f, 0.1762419641017914f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9800000190734863f, 0.2599999904632568f, 0.2599999904632568f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8799999952316284f, 0.2400000393390656f, 0.2400000393390656f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8669527769088745f, 0.6407000422477722f, 0.1823205500841141f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.8669527769088745f, 0.1785997450351715f, 0.1785997450351715f, 0.4000000059604645f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7424892783164978f, 0.0477997250854969f, 0.0477997250854969f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.9999997615814209f, 0.9999899864196777f, 0.4892703890800476f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.8283261656761169f, 0.1137615367770195f, 0.1137615367770195f, 0.3100000023841858f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.8927038908004761f, 0.1685792952775955f, 0.1685792952775955f, 0.800000011920929f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9764705896377563f, 0.73930823802948f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7490196228027344f, 0.09803923219442368f, 0.09803923219442368f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7490196228027344f, 0.09803923219442368f, 0.09803923219442368f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.5764706134796143f, 0.1764705777168274f, 0.1764705777168274f, 0.8619999885559082f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5364806652069092f, 0.4367817640304565f, 0.1427545249462128f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.4235294163227081f, 0.1333333551883698f, 0.1333333551883698f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.09012877941131592f, 0.09012877941131592f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.412013053894043f, 0.412013053894043f, 0.4120171666145325f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 0.9999992847442627f, 0.3605149984359741f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.9764705896377563f, 0.2588235437870026f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);


}

static std::unordered_map<std::string, void (*)()> themes = 
{{"default", DEFAULT_THEME}, {"windark theme", WINDARK_THEME}, {"dark red theme", DARK_RED_THEME}};

static std::pair<std::string, void (*)()> selected_theme = *themes.find("default");

std::unordered_map<std::string, void (*)()>& ThemeManager_get_themes()
{
	return themes;
}

std::pair<std::string, void (*)()>& ThemeManager_get_selected_theme()
{
	return selected_theme;
}

void ThemeManager_add_theme(std::string name, void (*func)())
{
	auto r = themes.insert({name, func});
	if (!r.second)
	{
		ImVec4* color = new ImVec4(ERROR[0], ERROR[1], ERROR[2], ERROR[3]);
		debug_window->LOG((std::string("[ThemeManager] ~ No se ha podido introducir ") + name + ", pon un nombre que no se repita.").c_str(), color);
		return;
	}

	debug_window->LOG((std::string("[ThemeManager] ~ Se ha añadido el tema ") + name + " con éxito!").c_str(), nullptr);
}

