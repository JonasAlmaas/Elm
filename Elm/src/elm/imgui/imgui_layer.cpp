#include "imgui_layer.h"

#include "elm/core/application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// Temporary
#include <GLFW/glfw3.h>

namespace elm {

	ImGuiLayer::ImGuiLayer(void)
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer(void)
	{
	}

	void ImGuiLayer::on_attach(void)
	{
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

		Application* app = Application::get();
		GLFWwindow *window = static_cast<GLFWwindow*>(app->get_window()->get_native_window());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::on_detach(void)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::on_event(Event& e)
	{
		if (!m_block_events) {
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		e.handled |= e.is_in_category(EventCategoryMouse) & io.WantCaptureMouse;
		e.handled |= e.is_in_category(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void ImGuiLayer::begin(void)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end(void)
	{
		auto app = Application::get();

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
