#pragma once

#include "elm/core/base.hpp"
#include "elm/core/event/event.hpp"
#include "elm/core/timestep.hpp"
#include <string>

namespace elm {

	struct layer
	{
		layer(const std::string& name = "Layer"): debug_name(name) {}

		virtual ~layer() = default;

		virtual void on_attach(void) {}
		virtual void on_detach(void) {}
		virtual void on_update(timestep ts) {}
		virtual void on_event(event& e) {}

		virtual void on_imgui_render(void) {}

		inline const std::string& get_name(void) const { return this->debug_name; }

	private:
		std::string debug_name;
	};
}
