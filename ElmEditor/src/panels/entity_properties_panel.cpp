#include "entity_properties_panel.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace elm {

	template<typename T, typename UIFUNCTION>
	static void draw_component(const std::string &name, entity entity, UIFUNCTION ui_func)
	{
		if (!entity.has_component<T>()) {
			return;
		}

		const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen
			| ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth
			| ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_FramePadding;

		auto &comp = entity.get_component<T>();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), tree_node_flags, name.c_str());
		ImGui::PopStyleVar();

		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		if (ImGui::Button("+##ComponentSettings")) {
			ImGui::OpenPopup("ComponentSettings");
		}
		ImGui::PopStyleVar();

		bool remove_comp = false;
		if (ImGui::BeginPopup("ComponentSettings")) {
			if (ImGui::MenuItem("Remove")) {
				remove_comp = true;
			}

			ImGui::EndPopup();
		}

		if (open) {
			ui_func(entity, comp);
			ImGui::TreePop();
		}

		if (remove_comp) {
			entity.remove_component<T>();
		}
	}

	template<typename T>
	static void display_add_component_entry(const std::string &label, entity entt)
	{
		if (!entt.has_component<T>()) {
			if (ImGui::MenuItem(label.c_str())) {
				entt.add_component<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void entity_properties_panel::on_imgui_render(editor_state &state)
	{
		ImGui::Begin("Entity Properties");

		if (state.selected_entity) {
			auto entt = state.selected_entity;

			// -- Tag --
			bool has_tag_comp = entt.has_component<tag_component>();
			auto &tag_comp = entt.has_component<tag_component>()
				? entt.get_component<tag_component>()
				: entt.add_component<tag_component>();
			char buf[50] = {0};
			if (has_tag_comp) {
				strcpy_s(buf, sizeof buf, tag_comp.tag.c_str());
			} else {
				strcpy_s(buf, sizeof buf, "Unknown entity");
				tag_comp.tag = std::string(buf);
			}
			if (ImGui::InputText("Tag##tag_component", buf, sizeof buf)) {
				tag_comp.tag = std::string(buf);
			}

			ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Button("+##AddComponent")) {
				ImGui::OpenPopup("AddComponent");
			}
			ImGui::PopStyleVar();

			if (ImGui::BeginPopup("AddComponent")) {
				display_add_component_entry<transform_component>("Add transform", entt);
				display_add_component_entry<sprite_renderer_component>("Add sprite renderer", entt);
				display_add_component_entry<circle_renderer_component>("Add circle renderer", entt);
				display_add_component_entry<text_renderer_component>("Add text renderer", entt);
				display_add_component_entry<environment_light_component>("Add environment light", entt);
				display_add_component_entry<directional_light_component>("Add directional light", entt);
				display_add_component_entry<point_light_component>("Add point light", entt);
				display_add_component_entry<mesh_renderer_component>("Add mesh renderer", entt);

				ImGui::EndPopup();
			}

			// -- Transform --
			draw_component<transform_component>("Transform", entt, [](entity entt, transform_component &comp) {
				glm::vec3 translation, rotation_rad, scale;
				math::decompose_transform(comp.transform, &translation, &rotation_rad, &scale);
				glm::vec3 rotation_deg = glm::degrees(rotation_rad);

				bool changed = false;
				if (ImGui::DragFloat3("Translation##transform_component", glm::value_ptr(translation), 0.01f)) changed = true;
				if (ImGui::DragFloat3("Rotation##transform_component", glm::value_ptr(rotation_deg), 0.1f)) changed = true;
				if (ImGui::DragFloat3("Scale##transform_component", glm::value_ptr(scale), 0.01f)) changed = true;

				if (changed) {
					comp.transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::toMat4(glm::quat(glm::radians(rotation_deg)))
						* glm::scale(glm::mat4(1.0f), scale);
				}
			});

			// -- Sprite renderer --
			draw_component<sprite_renderer_component>("Sprite renderer", entt, [](entity entt, sprite_renderer_component &comp) {
				ImGui::ColorEdit3("Color##sprite_renderer_component", glm::value_ptr(comp.color));
				// TODO: texture
				ImGui::DragFloat2("Tiling factor##sprite_renderer_component", glm::value_ptr(comp.tiling_factor), 0.001f);
			});

			// -- Circle renderer --
			draw_component<circle_renderer_component>("Circle renderer", entt, [](entity entt, circle_renderer_component &comp) {
				ImGui::ColorEdit3("Color##circle_renderer_component", glm::value_ptr(comp.color));
				ImGui::DragFloat("Radius##circle_renderer_component", &comp.radius, 0.01f);
				ImGui::DragFloat("Thickness##circle_renderer_component", &comp.thickness, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade##circle_renderer_component", &comp.fade, 0.001f);
			});

			// -- Text renderer --
			draw_component<text_renderer_component>("Text renderer", entt, [](entity entt, text_renderer_component &comp) {
				static char buf[1024];
				memset(buf, 0, sizeof buf);
				strcpy_s(buf, sizeof buf, comp.text.c_str()); // This can cause issues. Have fun with that :) -JA
				ImGui::InputTextMultiline("Text##text_renderer_component", buf, sizeof buf);
				// TODO: font
				ImGui::ColorEdit3("Color##text_renderer_component", glm::value_ptr(comp.color));
				ImGui::DragFloat("Kerning##text_renderer_component", &comp.kerning, 0.01f);
				ImGui::DragFloat("Line spacing##text_renderer_component", &comp.line_spacing, 0.01f);
			});

			// -- Environment light --
			draw_component<environment_light_component>("Environment light", entt, [](entity entt, environment_light_component &comp) {
				// TODO
			});

			// -- Directional light --
			draw_component<directional_light_component>("Directional light", entt, [](entity entt, directional_light_component &comp) {
				ImGui::DragFloat3("Direction##directional_light_component", glm::value_ptr(comp.direction), 0.01f);
				ImGui::ColorEdit3("Color##directional_light_component", glm::value_ptr(comp.color));
				ImGui::DragFloat("Intensity##directional_light_component", &comp.intensity, 0.01f);
			});

			// -- Point light --
			draw_component<point_light_component>("Point light", entt, [](entity entt, point_light_component &comp) {
				ImGui::ColorEdit3("Color##point_light_component", glm::value_ptr(comp.color));
				ImGui::DragFloat("Intensity##point_light_component", &comp.intensity, 0.01f);
			});

			// -- Mesh renderer --
			draw_component<mesh_renderer_component>("Mesh renderer", entt, [](entity entt, mesh_renderer_component &comp) {
				// TODO
			});
		}

		ImGui::End();
	}
}
