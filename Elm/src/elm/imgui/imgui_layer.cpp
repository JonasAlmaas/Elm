#include "imgui_layer.h"

#include "elm/core/application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Temporary
#include <GLFW/glfw3.h>

namespace elm {

	imgui_layer::imgui_layer(void)
		: layer("ImGuiLayer")
	{
	}

	imgui_layer::~imgui_layer(void)
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

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		application* app = application::get();
		GLFWwindow *window = static_cast<GLFWwindow*>(app->get_window()->get_native_window());

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

		if (!m_block_events) {
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		e.handled |= e.is_in_category(EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
		e.handled |= e.is_in_category(EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
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
}
