#pragma once

#include "elm/core/base.hpp"
#include <string>
#include <functional>

namespace elm {

	enum class event_type {
		EVENT_TYPE_NONE = 0,
		EVENT_TYPE_WINDOW_CLOSE, EVENT_TYPE_WINDOW_MINIMIZE, EVENT_TYPE_WINDOW_RESIZE,
		EVENT_TYPE_WINDOW_FOCUS, EVENT_TYPE_WINDOW_BLUR, EVENT_TYPE_WINDOW_MOVED,
		EVENT_TYPE_KEY_PRESSED, EVENT_TYPE_KEY_RELEASED, EVENT_TYPE_KEY_TYPED,
		EVENT_TYPE_MOUSE_BUTTON_PRESSED, EVENT_TYPE_MOUSE_BUTTON_RELEASED, EVENT_TYPE_MOUSE_MOVED, EVENT_TYPE_MOUSE_SCROLLED
	};

	enum event_category {
		EVENT_CATEGORY_NONE = 0,
		EVENT_CATEGORY_APPLICATION = 1<<0,
		EVENT_CATEGORY_INPUT = 1<<1,
		EVENT_CATEGORY_KEYBOARD = 1<<2,
		EVENT_CATEGORY_MOUSE = 1<<3,
		EVENT_CATEGORY_MOUSE_BUTTON = 1<<4,
	};

	#define EVENT_CLASS_TYPE(type) static event_type get_static_type(void) { return event_type::type; }\
		virtual event_type get_event_type(void) const override { return get_static_type(); }\
		virtual const char* get_name(void) const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags(void) const override { return category; }

	struct event
	{
		friend struct event_dispatcher;

		virtual ~event(void) = default;

		virtual event_type get_event_type(void) const = 0;
		virtual const char *get_name(void) const = 0;
		virtual int get_category_flags(void) const = 0;
		virtual std::string to_string(void) const { return get_name(); }

		bool is_in_category(event_category category) const
		{
			return get_category_flags() & category;
		}

	public:
		bool handled = false;
	};

	struct event_dispatcher
	{
		event_dispatcher(event& e): e(e) {}

		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (this->e.get_event_type() == T::get_static_type()) {
				this->e.handled |= func(static_cast<T&>(this->e));
				return true;
			}
			return false;
		}

	private:
		event& e;
	};
}

inline std::ostream& operator<<(std::ostream& os, const elm::event& e)
{
	return os << e.to_string();
}

template <> struct fmt::formatter<elm::event> : fmt::ostream_formatter {};
