#pragma once

#include "event.hpp"
#include <sstream>
#include <stdint.h>

namespace elm {

	struct window_close_event : event
	{
		window_close_event(void) = default;

		EVENT_CLASS_TYPE(ET_WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(ECAT_APPLICATION)
	};

	struct window_minimize_event : event
	{
		window_minimize_event(bool minimized)
			: minimized(minimized) {}

		inline bool is_minimized(void) const { return this->minimized; }

		EVENT_CLASS_TYPE(ET_WINDOW_MINIMIZE)
		EVENT_CLASS_CATEGORY(ECAT_APPLICATION)

	private:
		bool minimized;
	};

	struct window_resize_event : event
	{
		window_resize_event(uint32_t width, uint32_t height)
			: width(width), height(height) {}

		inline uint32_t get_width(void) const { return this->width; }
		inline uint32_t get_height(void) const { return this->height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << this->width << ", " << this->height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(ECAT_APPLICATION)

	private:
		uint32_t width, height;
	};
}
