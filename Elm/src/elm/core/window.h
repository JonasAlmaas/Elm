#pragma once

#include "elm/core/event/event.h"
#include <functional>
#include <string>
#include <stdint.h>

namespace elm {

	struct window_specification {
		std::string title = "Elm engine";
		uint32_t width = 1280u;
		uint32_t height = 720u;
		bool vsync = true;
	};

	class window
	{
	public:
		virtual ~window(void) = default;

		virtual void on_update(bool minimized) = 0;

		virtual uint32_t get_width(void) const = 0;
		virtual uint32_t get_height(void) const = 0;

		// Window attributes
		virtual void set_event_callback(const std::function<void(event &)> & cb) = 0;
		virtual void set_vsync(bool enabled) = 0;
		virtual bool is_vsync(void) const = 0;

		virtual void *get_native_window(void) = 0;

	public:
		static std::unique_ptr<window> create(const window_specification& spec = window_specification());
	};
}
