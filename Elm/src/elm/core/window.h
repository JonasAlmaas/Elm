#pragma once

#include "elm/core/event/event.h"
#include <functional>
#include <string>
#include <stdint.h>

namespace elm {

	struct WindowSpecification
	{
		std::string title = "Elm engine";
		uint32_t width = 1920u;
		uint32_t height = 1080u;
		bool vsync = true;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(void) = default;

		virtual void on_update(void) = 0;

		virtual uint32_t get_width(void) const = 0;
		virtual uint32_t get_height(void) const = 0;

		// Window attributes
		virtual void set_event_callback(const EventCallbackFn& cb) = 0;
		virtual void set_vsync(bool enabled) = 0;
		virtual bool is_vsync(void) const = 0;

		static Window* create(const WindowSpecification& spec = WindowSpecification());
	};
}
