#include "application.hpp"
#include "elm/core/time.hpp"
#include "elm/renderer/renderer.hpp"
#include "elm/random.hpp"

namespace elm {

	application* application::s_instance = nullptr;

	application::application(const struct application_specification &spec, struct application_command_line_args args)
		: spec(spec), cmd_line_args(args)
	{
		ELM_PROFILE_FUNCTION();

		ELM_CORE_ASSERT(!s_instance, "Application already exists");
		s_instance = this;

		if (!this->spec.cwd.empty()) {
			std::filesystem::current_path(this->spec.cwd);
		}

		this->window = window::create({
			.title = this->spec.name,
			.width = this->spec.window_width,
			.height = this->spec.window_height,
			.vsync = this->spec.vsync});
		this->window->set_event_callback(ELM_BIND_EVENT_FN(application::on_event));

		random::init();
		renderer::init();

		this->imgui_l = new imgui_layer();
		push_overlay(this->imgui_l);
	}

	void application::run(void)
	{
		ELM_PROFILE_FUNCTION();

		while (this->running) {
			ELM_PROFILE_SCOPE("application::run() - Run loop");

			float time = time::get_seconds();
			timestep timestep(time - this->last_frame_time_sec);
			this->last_frame_time_sec = time;

			if (!this->minimized) {
				this->telemetry.on_update(timestep);

				ELM_PROFILE_SCOPE("application::run() - Layer stack on_update()");
				for (auto layer : this->layer_stack) {
					layer->on_update(timestep);
				}
			}

			this->imgui_l->begin();
			{
				ELM_PROFILE_SCOPE("application::run() - Layer stack on_imgui_render()");
				for (auto layer : this->layer_stack) {
					layer->on_imgui_render();
				}
			}
			this->imgui_l->end();

			this->window->on_update(this->minimized);
		}

		renderer::shutdown();
	}

	void application::push_layer(layer* layer)
	{
		ELM_PROFILE_FUNCTION();

		this->layer_stack.push_layer(layer);
	}

	void application::push_overlay(layer* layer)
	{
		ELM_PROFILE_FUNCTION();

		this->layer_stack.push_overlay(layer);
	}

	void application::on_event(event& e)
	{
		ELM_PROFILE_FUNCTION();

		event_dispatcher dispatcher(e);
		dispatcher.dispatch<window_close_event>(ELM_BIND_EVENT_FN(application::on_window_close));
		dispatcher.dispatch<window_resize_event>(ELM_BIND_EVENT_FN(application::on_window_resize));

		for (auto it = this->layer_stack.end(); it != this->layer_stack.begin(); ) {
			if (e.handled) {
				break;
			}
			(*--it)->on_event(e);
		}
	}

	bool application::on_window_close(window_close_event &e)
	{
		ELM_PROFILE_FUNCTION();

		this->running = false;
		return true;
	}

	bool application::on_window_resize(window_resize_event &e)
	{
		ELM_PROFILE_FUNCTION();

		uint32_t width = e.get_width();
		uint32_t height = e.get_height();

		if (width == 0 || height == 0) {
			this->minimized = true;
			return true;
		}

		this->minimized = false;

		renderer::on_viewport_resize(width, height);

		return false;
	}
}
