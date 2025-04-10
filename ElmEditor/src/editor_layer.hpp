#pragma once

#include "panels/entity_properties_panel.hpp"
#include "panels/scene_properties_panel.hpp"
#include "panels/scene_hierarchy_panel.hpp"
#include "panels/shader_library_panel.hpp"
#include "panels/viewport_panel.hpp"
#include <elm.hpp>
#include <memory>

namespace elm {

	struct editor_layer : layer
	{
		editor_layer(void);
		virtual ~editor_layer(void) = default;

		virtual void on_attach(void) override;
		virtual void on_detach(void) override;
		virtual void on_update(timestep ts) override;
		virtual void on_event(event &e) override;
		virtual void on_imgui_render(void) override;

	private:
		std::shared_ptr<scene> scene;
		shader_library shader_lib;

		entity_properties_panel entity_properties_panel;
		scene_properties_panel scene_properties_panel;
		scene_hierarchy_panel scene_hierarchy_panel;
		shader_library_panel shader_library_panel;
		viewport_panel viewport_panel;
	};
}
