#pragma once

#include "elm/core/event/event.h"
#include <string>

namespace elm {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			: m_debug_name(name)
		{
		}

		virtual ~Layer() = default;

		virtual void on_attach(void) {}
		virtual void on_detach(void) {}
		virtual void on_update(void) {}
		virtual void on_event(Event& e) {}

		inline const std::string& get_name(void) const { return m_debug_name; }

	private:
		std::string m_debug_name;

	};
}
