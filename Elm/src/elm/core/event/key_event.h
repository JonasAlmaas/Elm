#pragma once

#include "event.h"
#include "elm/core/input/key_code.h"
#include <sstream>

namespace elm {

	class key_event : public event
	{
	public:
		inline key_code get_key_code() const { return m_key_code; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)

	protected:
		key_event(key_code keycode)
			: m_key_code(keycode)
		{
		}

		key_code m_key_code;
	};

	class key_pressed_event : public key_event
	{
	public:
		key_pressed_event(key_code keycode, bool is_repeat = false)
			: key_event(keycode), m_is_repeat(is_repeat)
		{
		}

		bool is_repeat() const { return m_is_repeat; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_key_code << " (repeat = " << m_is_repeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_KEY_PRESSED)

	private:
		bool m_is_repeat;
	};

	class key_released_event : public key_event
	{
	public:
		key_released_event(key_code keycode)
			: key_event(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_KEY_RELEASED)
	};

	class key_typed_event : public key_event
	{
	public:
		key_typed_event(key_code keycode)
			: key_event(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(EVENT_TYPE_KEY_TYPED)
	};
}
