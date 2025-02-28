#pragma once

#include "event.h"
#include "elm/core/input/key_code.h"
#include <sstream>

namespace elm {

	class KeyEvent : public Event
	{
	public:
		inline KeyCode get_key_code() const { return m_key_code; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(KeyCode keycode)
			: m_key_code(keycode)
		{
		}

		KeyCode m_key_code;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, bool is_repeat = false)
			: KeyEvent(keycode), m_is_repeat(is_repeat)
		{
		}

		bool IsRepeat() const { return m_is_repeat; }

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_key_code << " (repeat = " << m_is_repeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool m_is_repeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode)
		{
		}

		std::string to_string() const override
		{
			std::stringstream ss;
			ss << get_name() << ": " << m_key_code;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
