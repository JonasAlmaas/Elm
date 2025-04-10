#pragma once

#include "editor_state.hpp"
#include <elm.hpp>
#include <memory>

namespace elm {

	struct scene_hierarchy_panel
	{
		void on_imgui_render(std::shared_ptr<scene> &scene, editor_state &state);

	private:
		void draw_entity_node(entity ent, editor_state &state);
	};
}
