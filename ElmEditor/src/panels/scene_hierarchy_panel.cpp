#include "scene_hierarchy_panel.hpp"
#include <imgui.h>

namespace elm {

	void scene_hierarchy_panel::on_imgui_render(std::shared_ptr<scene> &scene, editor_state &state)
	{
		ImGui::Begin("Scene Hierarchy");

		auto &reg = scene->get_reg();
		for (auto entity_handle : reg.view<entt::entity>()) {
			draw_entity_node(entity(&reg, entity_handle), state);
		}

		ImGui::End();
	}

	void scene_hierarchy_panel::draw_entity_node(entity ent, editor_state &state)
	{
		std::string tag;
		if (ent.has_component<tag_component>()) {
			auto &tag_comp = ent.get_component<tag_component>();
			tag = tag_comp.tag;
		} else {
			tag = "Unknown entity";
		}

		ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_SpanAvailWidth;
		if (ent == state.selected_entity) {
			tree_node_flags |= ImGuiTreeNodeFlags_Selected;
		}

		// TODO: If the entity doesnt have any more children, add this flag
		// ImGuiTreeNodeFlags_Leaf
		tree_node_flags |= ImGuiTreeNodeFlags_Leaf; // We don't support a hierarchy for now

		bool open = ImGui::TreeNodeEx((void *)(uint64_t)ent.get_entity_handle(), tree_node_flags, tag.c_str());

		if (ImGui::IsItemClicked()) {
			if (ent == state.selected_entity) {
				state.selected_entity = entity();
			} else {
				state.selected_entity = ent;
			}
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete")) {
				if (ent == state.selected_entity) {
					state.selected_entity = entity();
				}
				ent.destroy();
			}

			ImGui::EndPopup();
		}

		if (open) {
			ImGui::TreePop();
		}
	}
}
