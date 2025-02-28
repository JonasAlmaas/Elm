#pragma once

#include "elm/core.h"

#include <string>
#include <functional>

namespace elm {

	enum class EventType {
		None = 0,
		WindowClose, WindowMinimize, WindowResize, WindowFocus, WindowBlur, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

	#define EVENT_CLASS_TYPE(type) static EventType get_static_type(void) { return EventType::##type; }\
		virtual EventType get_event_type(void) const override { return get_static_type(); }\
		virtual const char* get_name(void) const override { return #type; }\

	#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags(void) const override { return category; }

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event(void) = default;

		bool handled = false;

		virtual EventType get_event_type(void) const = 0;
		virtual const char *get_name(void) const = 0;
		virtual int get_category_flags(void) const = 0;
		virtual std::string to_string(void) const { return get_name(); }

		bool is_in_category(EventCategory category) const
		{
			return this->get_category_flags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e)
			: m_event(e)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_event.get_event_type() == T::get_static_type()) {
				m_event.handled |= func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.to_string();
	}
}
