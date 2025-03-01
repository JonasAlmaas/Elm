#pragma once

#include "event.h"
#include "elm/core/input/mouse_code.h"
#include <sstream>

namespace elm {

	class mouse_button_event : public event
	{
	public:
		inline mouse_code get_mouse_button(void) const { return m_button; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_MOUSE_BUTTON)

	protected:
		mouse_button_event(mouse_code button)
			: m_button(button)
		{
		}

		mouse_code m_button;
	};

	class mouse_button_pressed_event : public mouse_button_event
	{
	public:
		mouse_button_pressed_event(mouse_code button)
			: mouse_button_event(button)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_MOUSE_BUTTON_PRESSED)
	};

	class mouse_button_released_event : public mouse_button_event
	{
	public:
		mouse_button_released_event(mouse_code button)
			: mouse_button_event(button)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_MOUSE_BUTTON_RELEASED)
	};

	class mouse_moved_event : public event
	{
	public:
		mouse_moved_event(float x, float y)
			: m_mouse_x(x), m_mouse_y(y)
		{
		}

		inline float get_x() const { return m_mouse_x; }
		inline float get_y() const { return m_mouse_y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_mouse_x << ", " << m_mouse_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_MOUSE_MOVED)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)

	private:
		float m_mouse_x, m_mouse_y;
	};

	class mouse_scrolled_event : public event
	{
	public:
		mouse_scrolled_event(float offset_x, float offset_y)
			: m_offset_x(offset_x), m_offset_y(offset_y)
		{
		}

		inline float get_offset_x() const { return m_offset_x; }
		inline float get_offset_y() const { return m_offset_y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_offset_x << ", " << m_offset_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_MOUSE_SCROLLED)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)

	private:
		float m_offset_x, m_offset_y;
	};
}
