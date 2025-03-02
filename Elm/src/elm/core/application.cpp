#include "application.h"
#include "elm/core/renderer/renderer.h"

// Temp (used to get time)
#include <GLFW/glfw3.h>

namespace elm {

	application* application::s_instance = nullptr;

	application::application(void)
	{
		ELM_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = std::unique_ptr<window>(window::create());
		m_window->set_event_callback(ELM_BIND_EVENT_FN(application::on_event));

		renderer::init();

		m_imgui_layer = new imgui_layer();
		push_overlay(m_imgui_layer);
	}

	application::~application(void)
	{
	}

	void application::run(void)
	{
		while (m_running) {
			float time = (float)glfwGetTime();
			timestep timestep(time - m_last_frame_time_sec);
			m_last_frame_time_sec = time;

			if (!m_minimized) {
				for (auto layer : m_layer_stack) {
					layer->on_update(timestep);
				}
			}

			m_imgui_layer->begin();
			for (auto layer : m_layer_stack) {
				layer->on_imgui_render();
			}
			m_imgui_layer->end();

			m_window->on_update(m_minimized);
		}
	}

	void application::push_layer(layer* layer)
	{
		m_layer_stack.push_layer(layer);
	}

	void application::push_overlay(layer* layer)
	{
		m_layer_stack.push_overlay(layer);
	}

	void application::on_event(event& e)
	{
		event_dispatcher dispatcher(e);
		dispatcher.dispatch<window_close_event>(ELM_BIND_EVENT_FN(application::on_window_close));
		dispatcher.dispatch<window_resize_event>(ELM_BIND_EVENT_FN(application::on_window_resize));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			(*--it)->on_event(e);
			if (e.handled) {
				break;
			}
		}
	}

	bool application::on_window_close(window_close_event &e)
	{
		m_running = false;
		return true;
	}

	bool application::on_window_resize(window_resize_event &e)
	{
		uint32_t width = e.get_width();
		uint32_t height = e.get_height();

		if (width == 0 || height == 0) {
			m_minimized = true;
			return false;
		}

		m_minimized = false;

		renderer::on_window_resize(width, height);

		return false;
	}
}
