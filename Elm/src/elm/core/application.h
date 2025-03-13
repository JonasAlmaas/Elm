#pragma once

#include "elm/core/base.h"
#include "elm/core/event/event.h"
#include "elm/core/event/window_event.h"
#include "elm/core/layer_stack.h"
#include "elm/core/application_telemetry.h"
#include "elm/core/window.h"
#include "elm/imgui/imgui_layer.h"
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
			const struct application_specification &spec,
			struct application_command_line_args args = struct application_command_line_args());
		virtual ~application(void) = default;

		void run(void);

		inline void close(void) { m_running = false; }

		void push_layer(layer *layer);
		void push_overlay(layer *layer);

		inline struct application_command_line_args get_command_line_args(void) const { return m_cmd_line_args; };
		inline const application_telemetry *get_telemetry(void) const { return &m_telemetry; }
		inline imgui_layer *get_imgui_layer(void) const { return m_imgui_layer; }
		inline window *get_window(void) const { return m_window.get(); }

	public:
		static inline application *get(void) { return s_instance; }

	private:
		void on_event(event& e);
		bool on_window_close(window_close_event &e);
		bool on_window_resize(window_resize_event &e);

	private:
		struct application_specification m_spec;
		struct application_command_line_args m_cmd_line_args;

		bool m_running = true;
		bool m_minimized = false;
		std::unique_ptr<window> m_window;
		layer_stack m_layer_stack;
		imgui_layer *m_imgui_layer;

		float m_last_frame_time_sec = 0.0f;

		application_telemetry m_telemetry;

	private:
		static application *s_instance;

	public:
		// To be defined by the client
		static application *create(struct application_command_line_args args);
	};
}
