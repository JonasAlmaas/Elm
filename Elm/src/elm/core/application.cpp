#include "application.h"
#include "elm/renderer/renderer.h"
#include "elm/platform_utils/time.h"

namespace elm {

	application* application::s_instance = nullptr;

	application::application(const struct application_specification &spec, struct application_command_line_args args)
		: m_spec(spec), m_command_line_args(args)
	{
		ELM_PROFILE_FUNCTION();

		ELM_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		m_window = window::create({
			.title = m_spec.name,
			.width = m_spec.window_width,
			.height = m_spec.window_height,
			.vsync = m_spec.vsync});
		m_window->set_event_callback(ELM_BIND_EVENT_FN(application::on_event));

		renderer::init();

		m_imgui_layer = new imgui_layer();
		push_overlay(m_imgui_layer);
	}

	void application::run(void)
	{
		ELM_PROFILE_FUNCTION();

		while (m_running) {
			ELM_PROFILE_SCOPE("application::run() - Run loop");

			float time = time::get_seconds();
			timestep timestep(time - m_last_frame_time_sec);
			m_last_frame_time_sec = time;

			if (!m_minimized) {
				m_telemetry.on_update(timestep);

				ELM_PROFILE_SCOPE("application::run() - Layer stack on_update()");
				for (auto layer : m_layer_stack) {
					layer->on_update(timestep);
				}
			}

			m_imgui_layer->begin();
			{
				ELM_PROFILE_SCOPE("application::run() - Layer stack on_imgui_render()");
				for (auto layer : m_layer_stack) {
					layer->on_imgui_render();
				}
			}
			m_imgui_layer->end();

			m_window->on_update(m_minimized);
		}
	}

	void application::push_layer(layer* layer)
	{
		ELM_PROFILE_FUNCTION();

		m_layer_stack.push_layer(layer);
	}

	void application::push_overlay(layer* layer)
	{
		ELM_PROFILE_FUNCTION();

		m_layer_stack.push_overlay(layer);
	}

	void application::on_event(event& e)
	{
		ELM_PROFILE_FUNCTION();

		event_dispatcher dispatcher(e);
		dispatcher.dispatch<window_close_event>(ELM_BIND_EVENT_FN(application::on_window_close));
		dispatcher.dispatch<window_resize_event>(ELM_BIND_EVENT_FN(application::on_window_resize));

		for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); ) {
			if (e.handled) {
				break;
			}
			(*--it)->on_event(e);
		}
	}

	bool application::on_window_close(window_close_event &e)
	{
		ELM_PROFILE_FUNCTION();

		m_running = false;
		return true;
	}

	bool application::on_window_resize(window_resize_event &e)
	{
		ELM_PROFILE_FUNCTION();

		uint32_t width = e.get_width();
		uint32_t height = e.get_height();

		if (width == 0 || height == 0) {
			m_minimized = true;
			return false;
		}

		m_minimized = false;

		renderer::on_viewport_resize(width, height);

		return false;
	}
}
