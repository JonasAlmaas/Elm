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

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(timestep ts) {}
		virtual void on_event(event& e) {}

		virtual void on_imgui_render() {}

		const std::string& get_name() const {return this->debug_name;}

	private:
		std::string debug_name;
	};
}
