#include "imgui_layer.h"

#include "elm/core/application.h"
#include "elm/platform/opengl/imgui_opengl_renderer.h"
#include <imgui.h>

// Temporary
#include <GLFW/glfw3.h>

namespace elm {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::on_attach(void)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;				// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;				// Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		/*Application* app = Application::get();
		GLFWwindow *window = static_cast<GLFWwindow*>(app->get_window()->get_native_window());

		ImGui_ImplGlfw_InitForOpenGL(window, true);*/
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::on_detach(void)
	{
		ImGui_ImplOpenGL3_Shutdown();
		/*ImGui_ImplGlfw_Shutdown(); */
		ImGui::DestroyContext();
	}

	void ImGuiLayer::on_update(void)
	{
		auto app = Application::get();

		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(
			(float)app->get_window()->get_width(),
			(float)app->get_window()->get_height());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
}
