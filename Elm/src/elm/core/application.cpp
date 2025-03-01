#include "application.h"

#include <glad/glad.h>

namespace elm {

	Application* Application::s_instance = nullptr;

	Application::Application(void)
		: m_running(true)
	{
		ELM_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(BIND_EVENT_FN(Application::on_event));

		m_imgui_layer = new ImGuiLayer();
		push_overlay(m_imgui_layer);
	}

	Application::~Application(void)
	{
	}

	void Application::run(void)
	{
		while (m_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto layer : m_layer_stack) {
				layer->on_update();
			}

			m_imgui_layer->begin();
			for (auto layer : m_layer_stack) {
				layer->on_imgui_render();
			}
			m_imgui_layer->end();

			m_window->on_update();
		}
	}

	void Application::push_layer(Layer* layer)
	{
		m_layer_stack.push_layer(layer);
	}

	void Application::push_overlay(Layer* layer)
	{
		m_layer_stack.push_overlay(layer);
	}

	void Application::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::on_window_close));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*--it)->on_event(e);
			if (e.handled) {
				break;
			}
		}
	}

	bool Application::on_window_close(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}
