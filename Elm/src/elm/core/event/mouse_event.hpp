#pragma once

#include "event.hpp"
#include "elm/core/input.hpp"
#include <sstream>

namespace elm {

	struct mouse_button_event : event
	{
		mouse get_mouse_button() const {return this->button;}

		EVENT_CLASS_CATEGORY(ECAT_INPUT | ECAT_MOUSE | ECAT_MOUSE_BUTTON)

	protected:
		mouse_button_event(mouse button)
			: button(button) {}

		mouse button;
	};

	struct mouse_button_pressed_event : mouse_button_event
	{
		mouse_button_pressed_event(mouse button)
			: mouse_button_event(button) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << (uint16_t)this->button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_MOUSE_BUTTON_PRESSED)
	};

	struct mouse_button_released_event : mouse_button_event
	{
		mouse_button_released_event(mouse button)
			: mouse_button_event(button) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << (uint16_t)this->button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_MOUSE_BUTTON_RELEASED)
	};

	struct mouse_moved_event : event
	{
		mouse_moved_event(float x, float y)
			: mouse_x(x), mouse_y(y) {}

		float get_x() const { return this->mouse_x; }
		float get_y() const { return this->mouse_y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << this->mouse_x << ", " << this->mouse_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_MOUSE_MOVED)
		EVENT_CLASS_CATEGORY(ECAT_INPUT | ECAT_MOUSE)

	private:
		float mouse_x, mouse_y;
	};

	struct mouse_scrolled_event : event
	{
		mouse_scrolled_event(float offset_x, float offset_y)
			: offset_x(offset_x), offset_y(offset_y) {}

		float get_offset_x() const { return this->offset_x; }
		float get_offset_y() const { return this->offset_y; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << this->offset_x << ", " << this->offset_y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_MOUSE_SCROLLED)
		EVENT_CLASS_CATEGORY(ECAT_INPUT | ECAT_MOUSE)

	private:
		float offset_x, offset_y;
	};
}
