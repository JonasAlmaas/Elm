#pragma once

#include "elm/core/base.h"
#include "elm/core/event/event.h"
#include "elm/core/event/window_event.h"
#include "elm/core/layer_stack.h"
#include "elm/core/window.h"
#include "elm/imgui/imgui_layer.h"
#include <memory>

namespace elm {

	class Application
	{
	public:
		Application(void);
		virtual ~Application(void);

		// To be defined by the client
		static Application *Create(void);

		void run(void);

		void push_layer(Layer *layer);
		void push_overlay(Layer *layer);

		inline Window* get_window(void) const { return &*m_window; }

	public:
		static inline Application *get(void) { return s_instance; }

	private:
		void on_event(Event& e);
		bool on_window_close(WindowCloseEvent& e);

	private:
		bool m_running;
		std::unique_ptr<Window> m_window;
		LayerStack m_layer_stack;
		ImGuiLayer *m_imgui_layer;

		uint32_t m_vertex_array, m_vertex_buffer, m_index_buffer;

	private:
		static Application *s_instance;
	};
}
