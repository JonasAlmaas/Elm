#pragma once

#include "event.hpp"
#include "elm/core/input/key_code.hpp"
#include <sstream>

namespace elm {

	struct key_event : event
	{
		inline key_code get_key_code() const { return this->keycode; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)

	protected:
		key_event(key_code keycode)
			: keycode(keycode) {}

		key_code keycode;
	};

	struct key_pressed_event : key_event
	{
		key_pressed_event(key_code keycode, bool is_repeat=false)
			: key_event(keycode), is_repeating(is_repeat) {}

		bool is_repeat() const { return this->is_repeating; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << this->keycode << " (repeat = " << this->is_repeating << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_KEY_PRESSED)

	private:
		bool is_repeating;
	};

	struct key_released_event : key_event
	{
		key_released_event(key_code keycode)
			: key_event(keycode) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << this->keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_KEY_RELEASED)
	};

	struct key_typed_event : key_event
	{
		key_typed_event(key_code keycode)
			: key_event(keycode) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << this->keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_KEY_TYPED)
	};
}
