#pragma once

#include "event.hpp"
#include "elm/core/input.hpp"
#include <sstream>

namespace elm {

	struct key_event : event
	{
		key get_key_code() const { return this->keycode; }

		EVENT_CLASS_CATEGORY(ECAT_KEYBOARD | ECAT_INPUT)

	protected:
		key_event(key keycode)
			: keycode(keycode) {}

		key keycode;
	};

	struct key_pressed_event final : key_event
	{
		key_pressed_event(key keycode, bool is_repeat=false)
			: key_event(keycode), is_repeating(is_repeat) {}

		bool is_repeat() const { return this->is_repeating; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << (uint16_t)this->keycode << " (repeat = " << this->is_repeating << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_KEY_PRESSED)

	private:
		bool is_repeating;
	};

	struct key_released_event final : key_event
	{
		key_released_event(key keycode)
			: key_event(keycode) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << (uint16_t)this->keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_KEY_RELEASED)
	};

	struct key_typed_event final : key_event
	{
		key_typed_event(key keycode)
			: key_event(keycode) {}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << (uint16_t)this->keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ET_KEY_TYPED)
	};
}
