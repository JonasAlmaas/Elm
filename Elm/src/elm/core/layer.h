#pragma once

#include "elm/core/event/event.h"
#include "elm/core/timestep.h"
#include <string>

namespace elm {

	class layer
	{
	public:
		layer(const std::string& name = "Layer")
			: m_debug_name(name)
		{
		}

		virtual ~layer() = default;

		virtual void on_attach(void) {}
		virtual void on_detach(void) {}
		virtual void on_update(timestep ts) {}
		virtual void on_event(event& e) {}

		virtual void on_imgui_render(void) {}

		inline const std::string& get_name(void) const { return m_debug_name; }

	private:
		std::string m_debug_name;

	};
}
