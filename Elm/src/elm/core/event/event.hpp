#pragma once

#include "elm/core/base.hpp"
#include <string>
#include <functional>

namespace elm {

	enum class event_type {
		ET_NONE = 0,
		ET_WINDOW_CLOSE, ET_WINDOW_MINIMIZE, ET_WINDOW_RESIZE,
		ET_WINDOW_FOCUS, ET_WINDOW_BLUR, ET_WINDOW_MOVED,
		ET_KEY_PRESSED, ET_KEY_RELEASED, ET_KEY_TYPED,
		ET_MOUSE_BUTTON_PRESSED, ET_MOUSE_BUTTON_RELEASED, ET_MOUSE_MOVED, ET_MOUSE_SCROLLED
	};

	enum event_category {
		ECAT_NONE = 0,
		ECAT_APPLICATION = 1<<0,
		ECAT_INPUT = 1<<1,
		ECAT_KEYBOARD = 1<<2,
		ECAT_MOUSE = 1<<3,
		ECAT_MOUSE_BUTTON = 1<<4,
	};

	#define EVENT_CLASS_TYPE(type) \
		static event_type get_static_type() {return event_type::type;}\
		virtual event_type get_event_type() const override {return get_static_type();}\
		virtual const char* get_name() const override {return #type;}

	#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override {return category;}

	struct event
	{
		virtual ~event() = default;

		virtual event_type get_event_type() const = 0;
		virtual const char *get_name() const = 0;
		virtual int get_category_flags() const = 0;
		virtual std::string to_string() const {return get_name();}

		bool is_in_category(event_category category) const
		{
			return get_category_flags() & category;
		}

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
