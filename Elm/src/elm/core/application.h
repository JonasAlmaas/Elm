#pragma once

#include "elm/core/base.h"
#include "elm/core/event/event.h"
#include "elm/core/event/window_event.h"
#include "elm/core/layer_stack.h"
#include "elm/core/window.h"
#include "elm/imgui/imgui_layer.h"
#include <memory>

namespace elm {

	class application
	{
	public:
		application(void);
		virtual ~application(void) = default;

		// To be defined by the client
		static application *create(void);

		void run(void);

		void push_layer(layer *layer);
		void push_overlay(layer *layer);

		inline window* get_window(void) const { return m_window.get(); }

	public:
		static inline application *get(void) { return s_instance; }

	private:
		void on_event(event& e);
		bool on_window_close(window_close_event &e);
		bool on_window_resize(window_resize_event &e);

	private:
		bool m_running = true;
		bool m_minimized = false;
		std::unique_ptr<window> m_window;
		layer_stack m_layer_stack;
		imgui_layer *m_imgui_layer;

		float m_last_frame_time_sec = 0.0f;

	private:
		static application *s_instance;
	};
}
