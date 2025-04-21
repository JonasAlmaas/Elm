#pragma once

#include "elm/core/base.hpp"
#include "elm/core/event/event.hpp"
#include "elm/core/event/window_event.hpp"
#include "elm/core/layer_stack.hpp"
#include "elm/core/application_telemetry.hpp"
#include "elm/core/window.hpp"
#include "elm/imgui/imgui_layer.hpp"
#include <filesystem>
#include <memory>
#include <string>

namespace elm {

	struct application_command_line_args {
		int count = 0;
		char **args = nullptr;

		const char *operator[](int ix) const
		{
			ELM_CORE_ASSERT(ix < count, "Index out of range");
			return args[ix];
		}
	};

	struct application_specification {
		std::string name = "Elm";
		std::filesystem::path cwd = "";
		uint32_t window_width = 1280u;
		uint32_t window_height = 720u;
		bool vsync = true;
	};

	struct application
	{
		application(
			const application_specification &spec,
			application_command_line_args args = application_command_line_args());
		virtual ~application() = default;

		void run();

		void close() {this->running = false;}

		void push_layer(layer *layer);
		void push_overlay(layer *layer);

		application_command_line_args get_command_line_args() const {return this->cmd_line_args;};
		const application_telemetry *get_telemetry() const {return &this->telemetry;}
		imgui_layer *get_imgui_layer() const {return this->imgui_l;}
		window *get_window() const {return this->window.get();}

	public:
		static application *get() {return s_instance;}

	private:
		void on_event(event& e);
		bool on_window_close(window_close_event &e);
		bool on_window_resize(window_resize_event &e);

	private:
		application_specification spec;
		application_command_line_args cmd_line_args;

		bool running = true;
		bool minimized = false;
		std::unique_ptr<window> window;
		layer_stack layer_stack;
		imgui_layer *imgui_l;

		float last_frame_time_sec = 0.0f;

		application_telemetry telemetry;

	private:
		static application *s_instance;

	public:
		// To be defined by the client
		static application *create(application_command_line_args args);
	};
}
