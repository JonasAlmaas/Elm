#include "scene_properties_panel.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace elm {

	void scene_properties_panel::on_imgui_render(const std::shared_ptr<scene> &scene)
	{
		ImGui::Begin("Scene Properties");

		auto clear_color = scene->get_clear_color();
		if (ImGui::ColorEdit4("Clear color", glm::value_ptr(clear_color))) {
			scene->set_clear_color(clear_color);
		}

		bool show_world_grid = scene->get_show_world_grid();
		if (ImGui::Checkbox("Show world grid", &show_world_grid)) {
			scene->set_show_world_grid(show_world_grid);
		}

		ImGui::End();
	}
}
