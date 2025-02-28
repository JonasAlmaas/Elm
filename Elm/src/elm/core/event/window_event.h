#pragma once

#include "event.h"
#include <sstream>
#include <stdint.h>

namespace elm {

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(void) = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent(bool minimized)
			: m_minimized(minimized) {}

		inline bool is_minimized(void) const { return m_minimized; }

		EVENT_CLASS_TYPE(WindowMinimize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		bool m_minimized;
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_width(width), m_height(height) {}

		inline uint32_t get_width(void) const { return m_width; }
		inline uint32_t get_height(void) const { return m_height; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss <<  get_name() << ": " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		uint32_t m_width, m_height;
	};
}
