#pragma once

#include "event.h"
#include "elm/core/input/mouse_code.h"
#include <sstream>

namespace elm {

	class MouseButtonEvent : public Event
	{
	public:
		inline MouseCode get_mouse_button(void) const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(MouseCode button)
			: m_button(button)
		{
		}

		MouseCode m_button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button)
			: MouseButtonEvent(button)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button)
			: MouseButtonEvent(button)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
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

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		float m_mouse_x, m_mouse_y;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offset_x, float offset_y)
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

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		float m_offset_x, m_offset_y;
	};
}
