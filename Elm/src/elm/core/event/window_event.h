#pragma once

#include "event.h"
#include <sstream>
#include <stdint.h>

namespace elm {

	struct window_close_event : event
	{
		window_close_event(void) = default;

		EVENT_CLASS_TYPE(EVENT_TYPE_WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)
	};

	struct window_minimize_event : event
	{
		window_minimize_event(bool minimized)
			: m_minimized(minimized) {}

		inline bool is_minimized(void) const { return m_minimized; }

		EVENT_CLASS_TYPE(EVENT_TYPE_WINDOW_MINIMIZE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)

	private:
		bool m_minimized;
	};

	struct window_resize_event : event
	{
		window_resize_event(uint32_t width, uint32_t height)
			: m_width(width), m_height(height) {}

		inline uint32_t get_width(void) const { return m_width; }
		inline uint32_t get_height(void) const { return m_height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION)

	private:
		uint32_t m_width, m_height;
	};
}
