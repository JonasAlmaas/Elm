#include "editor_layer.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace elm {

	editor_layer::editor_layer(void)
		: layer("Editor Layer")
	{
	}

	void editor_layer::on_attach(void)
	{
		this->viewport_panel.on_attach();

		this->scene = scene::create();
		this->scene->set_clear_color({0.1f, 0.1f, 0.1f, 1.0f});
		this->scene->set_show_world_grid(true);

		auto pbr_shader = this->shader_lib.load("content/shaders/pbr.glsl");

		std::shared_ptr<texture_2d> texture_white;
		std::shared_ptr<texture_2d> texture_black;
		{
			texture_specification spec;
			spec.format = image_format::RGBA8;
			spec.mag_filter = texture_filter::NEAREST;

			texture_white = texture_2d::create(spec);
			texture_black = texture_2d::create(spec);
			{
				uint32_t texture_data = 0xFFFFFFFFu;
				texture_white->set_data(&texture_data, sizeof texture_data);
			}
			{
				uint32_t texture_data = 0xFF000000u;
				texture_black->set_data(&texture_data, sizeof texture_data);
			}
		}

		{ // Environment light
			entity entity = this->scene->create_entity();

			auto &tag = entity.add_component<tag_component>();
			tag.tag = "Environment light";

			auto& env = entity.add_component<environment_light_component>();
			auto cubemap = cubemap::create("content/textures/skybox/minedump_flats.hdr", 512);
			env.irradiance_map = cubemap::create_irradiance(cubemap, 32);
			env.prefilter_map = cubemap::create_prefilter(cubemap, 128);
			env.brdf_lut_map = cubemap::create_brdf_lut_map(512);
		}

		{ // Directional Light
			entity entity = this->scene->create_entity();

			auto &tag = entity.add_component<tag_component>();
			tag.tag = "Directional Light";

			auto &dir_light = entity.add_component<directional_light_component>();
			dir_light.direction = glm::normalize(glm::vec3(1, -1, -1));
			dir_light.color = {1.0f, 0.79f, 0.56f};
			dir_light.intensity = 1.0f;
		}

		{ // Point light
			entity entity = this->scene->create_entity();

			auto &tag = entity.add_component<tag_component>();
			tag.tag = "Point light";

			auto &light = entity.add_component<point_light_component>();
			light.color = {1.0f, 0.79f, 0.56f};
			light.intensity = 10.0f;

			auto &tc = entity.add_component<transform_component>();
			tc.transform = glm::translate(glm::mat4(1.0f), {-4.0f, -4.0f, 2.0f});
		}

		{ // Cube
			entity entity = this->scene->create_entity();

			auto &tag = entity.add_component<tag_component>();
			tag.tag = "Cube";

			auto &renderer = entity.add_component<mesh_renderer_component>();
			renderer.mesh = mesh::create("content/meshes/cube.obj");
			auto mat = std::make_shared<pbr_material>();
			mat->shader = pbr_shader;
			mat->albedo = texture_white;
			mat->normal = texture_white; // TODO: Use a proper normal texture
			mat->roughness = texture_white;
			mat->ao = texture_white;
			mat->metalness = texture_black;
			renderer.material = mat;

			auto &transform_comp = entity.add_component<transform_component>();
			transform_comp.transform = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.5f});
		}
	}

	void editor_layer::on_detach(void)
	{
	}

	void editor_layer::on_update(timestep ts)
	{
		this->viewport_panel.on_update(ts, this->scene);
	}

	void editor_layer::on_event(event &e)
	{
		this->viewport_panel.on_event(e);
	}

	void editor_layer::on_imgui_render(void)
	{
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Exit", "ALT+F4")) {
					application::get()->close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		this->entity_properties_panel.on_imgui_render(this->state);
		this->renderer_info_panel.on_imgui_render();
		this->scene_properties_panel.on_imgui_render(this->scene);
		this->scene_hierarchy_panel.on_imgui_render(this->scene, this->state);
		this->shader_library_panel.on_imgui_render(this->shader_lib);
		this->viewport_panel.on_imgui_render();
	}
}
