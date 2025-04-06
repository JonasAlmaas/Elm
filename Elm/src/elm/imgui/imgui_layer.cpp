#include "imgui_layer.hpp"

#include "elm/core/application.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Emedded font
#include "fonts/roboto/roboto-bold.embed"
#include "fonts/roboto/roboto-regular.embed"

// Temporary
#include <GLFW/glfw3.h>

namespace elm {

	imgui_layer::imgui_layer(void)
		: layer("ImGuiLayer")
	{
	}

	void imgui_layer::on_attach(void)
	{
		ELM_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		set_style();

		application *app = application::get();
		GLFWwindow *window = static_cast<GLFWwindow *>(app->get_window()->get_native_window());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void imgui_layer::on_detach(void)
	{
		ELM_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void imgui_layer::on_event(event& e)
	{
		ELM_PROFILE_FUNCTION();

		if (!this->block_events) {
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		e.handled |= e.is_in_category(ECAT_MOUSE) & io.WantCaptureMouse;
		e.handled |= e.is_in_category(ECAT_KEYBOARD) & io.WantCaptureKeyboard;
	}

	void imgui_layer::begin(void)
	{
		ELM_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void imgui_layer::end(void)
	{
		ELM_PROFILE_FUNCTION();

		auto app = application::get();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(
			(float)app->get_window()->get_width(),
			(float)app->get_window()->get_height());

		// Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void imgui_layer::set_style(void)
	{
		ELM_PROFILE_FUNCTION();

		ImGui::StyleColorsDark();

		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle &style = ImGui::GetStyle();

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(10.0f, 10.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(10.0f, 10.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(10.0f, 3.0f);
		style.FrameRounding = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(10.0f, 8.0f);
		style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.IndentSpacing = 10.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 10.0f;
		style.ScrollbarRounding = 100.0f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 0.0f;
		style.LogSliderDeadzone = 4.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
		style.DisplayWindowPadding = ImVec2(19.0f, 19.0f);
		style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
		style.MouseCursorScale = 1.0f;
		style.AntiAliasedLines = true;
		style.AntiAliasedLinesUseTex = true;
		style.AntiAliasedFill = true;
		style.CurveTessellationTol = 1.25f;
		style.CircleTessellationMaxError = 0.5f;

		style.Colors[ImGuiCol_Text] = { 0.95f, 0.95f, 0.95f, 1.0f };
		style.Colors[ImGuiCol_TextDisabled] = { 0.8f, 0.8f, 0.8f, 1.0f };

		style.Colors[ImGuiCol_WindowBg] = { 0.16f, 0.16f, 0.16f, 1.0f }; // Background of normal windows
		style.Colors[ImGuiCol_ChildBg] = { 0.16f, 0.16f, 0.16f, 1.0f }; // Background of child windows
		style.Colors[ImGuiCol_PopupBg] = { 0.16f, 0.16f, 0.16f, 1.0f }; // Background of popups, menus, tooltips windows

		style.Colors[ImGuiCol_Border] = { 0.22f, 0.22f, 0.22f, 1.0f };
		style.Colors[ImGuiCol_BorderShadow] = { 0.0f, 0.0f, 0.0f, 0.0f };

		style.Colors[ImGuiCol_FrameBg] = { 0.24f, 0.24f, 0.24f, 1.0f }; // Background of checkbox, radio button, plot, slider, text input
		style.Colors[ImGuiCol_FrameBgHovered] = { 0.29f, 0.29f, 0.29f, 1.0f };
		style.Colors[ImGuiCol_FrameBgActive] = { 0.24f, 0.24f, 0.24f, 1.0f };;

		style.Colors[ImGuiCol_TitleBg] = { 0.1f, 0.1f, 0.1f, 1.0f };
		style.Colors[ImGuiCol_TitleBgActive] = { 0.1f, 0.1f, 0.1f, 1.0f };
		style.Colors[ImGuiCol_TitleBgCollapsed] = { 0.1f, 0.1f, 0.1f, 1.0f };
		style.Colors[ImGuiCol_MenuBarBg] = { 0.1f, 0.1f, 0.1f, 1.0f };

		style.Colors[ImGuiCol_ScrollbarBg] = { 0.14f, 0.14f, 0.14f, 1.0f };
		style.Colors[ImGuiCol_ScrollbarGrab] = { 0.43f, 0.43f, 0.43f, 1.0f };
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.58f, 0.58f, 0.58f, 1.0f };
		style.Colors[ImGuiCol_ScrollbarGrabActive] = { 0.58f, 0.58f, 0.58f, 1.0f };

		style.Colors[ImGuiCol_CheckMark] = { 0.18f, 0.4f, 0.58f, 1.0f };
		style.Colors[ImGuiCol_SliderGrab] = { 0.18f, 0.4f, 0.58f, 1.0f };
		style.Colors[ImGuiCol_SliderGrabActive] = { 0.18f, 0.33f, 0.47f, 1.0f };

		style.Colors[ImGuiCol_Button] = { 0.24f, 0.24f, 0.24f, 1.0f };;
		style.Colors[ImGuiCol_ButtonHovered] = { 0.29f, 0.29f, 0.29f, 1.0f };
		style.Colors[ImGuiCol_ButtonActive] = { 0.24f, 0.24f, 0.24f, 1.0f };;

		style.Colors[ImGuiCol_Header] = { 0.25f, 0.25f, 0.25f, 1.0f }; // Header colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
		style.Colors[ImGuiCol_HeaderHovered] = { 0.25f, 0.25f, 0.25f, 1.0f };
		style.Colors[ImGuiCol_HeaderActive] = { 0.25f, 0.25f, 0.25f, 1.0f };

		style.Colors[ImGuiCol_Separator] = { 0.22f, 0.22f, 0.22f, 1.0f };
		style.Colors[ImGuiCol_SeparatorHovered] = { 0.22f, 0.22f, 0.22f, 1.0f };
		style.Colors[ImGuiCol_SeparatorActive] = { 0.22f, 0.22f, 0.22f, 1.0f };

		style.Colors[ImGuiCol_ResizeGrip] = { 0.25f, 0.25f, 0.25f, 1.0f };
		style.Colors[ImGuiCol_ResizeGripHovered] = { 0.18f, 0.33f, 0.47f, 1.0f };
		style.Colors[ImGuiCol_ResizeGripActive] = { 0.18f, 0.33f, 0.47f, 1.0f };

		style.Colors[ImGuiCol_Tab] = { 0.1f, 0.1f, 0.1f, 1.0f };
		style.Colors[ImGuiCol_TabHovered] = { 0.22f, 0.22f, 0.22f, 1.0f };
		style.Colors[ImGuiCol_TabActive] = { 0.22f, 0.22f, 0.22f, 1.0f };
		style.Colors[ImGuiCol_TabUnfocused] = { 0.1f, 0.1f, 0.1f, 1.0f };
		style.Colors[ImGuiCol_TabUnfocusedActive] = { 0.22f, 0.22f, 0.22f, 1.0f };

		style.Colors[ImGuiCol_DockingPreview] = { 0.17f, 0.17f , 0.17f, 1.0f}; // Preview overlay color when about to docking something
		style.Colors[ImGuiCol_DockingEmptyBg] = { 0.19f, 0.19f , 0.19f, 1.0f }; // Background color for empty node (e.g. CentralNode with no window docked into it)

		style.Colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.0f };
		style.Colors[ImGuiCol_PlotLinesHovered] = { 1.0f, 0.43f, 0.35f, 1.0f };
		style.Colors[ImGuiCol_PlotHistogram] = { 0.9f, 0.7f, 0.0f, 1.0f };
		style.Colors[ImGuiCol_PlotHistogramHovered] = { 1.0f, 0.6f, 0.0f, 1.0f };

		style.Colors[ImGuiCol_TableHeaderBg] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Table header background
		style.Colors[ImGuiCol_TableBorderStrong] = { 0.25f, 0.25f, 0.25f, 1.0f }; // Table outer and header borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableBorderLight] = { 0.2f, 0.2f, 0.2f, 1.0f }; // Table inner borders (prefer using Alpha=1.0 here)
		style.Colors[ImGuiCol_TableRowBg] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Table row background (even rows)
		style.Colors[ImGuiCol_TableRowBgAlt] = { 1.0f, 1.0f, 1.0f, 0.03f }; // Table row background (odd rows)

		style.Colors[ImGuiCol_TextSelectedBg] = { 0.18f, 0.33f, 0.47f, 1.0f };

		style.Colors[ImGuiCol_DragDropTarget] = { 0.33f, 0.33f, 0.33f, 1.0f };
		style.Colors[ImGuiCol_NavHighlight] = { 0.33f, 0.33f, 0.33f, 1.0f }; // Gamepad/keyboard: current highlighted item

		style.Colors[ImGuiCol_NavWindowingHighlight] = { 0.33f, 0.33f, 0.33f, 1.0f }; // Highlight window when using CTRL+TAB
		style.Colors[ImGuiCol_NavWindowingDimBg] = { 0.8f, 0.8f, 0.8f, 0.2f }; // Darken/colorize entire screen behind the CTRL+TAB window list, when active
		style.Colors[ImGuiCol_ModalWindowDimBg] = { 0.8f, 0.8f, 0.8f, 0.35f }; // Darken/colorize entire screen behind a modal window, when one is active

		ImFontConfig font_config;
		font_config.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF((void *)g_roboto_bold, sizeof g_roboto_bold, 18.0f, &font_config);

		ImFont *roboto_font = io.Fonts->AddFontFromMemoryTTF((void *)g_roboto_regular, sizeof g_roboto_regular, 18.0f, &font_config);
		io.FontDefault = roboto_font;
	}
}
