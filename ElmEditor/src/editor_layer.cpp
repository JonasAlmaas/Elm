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
		}

		{ // Environment light
			auto cubemap = cubemap::create("content/textures/skybox/minedump_flats.hdr", 512);
			entity entity = this->scene->create_entity();
			auto& env = entity.add_component<environment_light_component>();
			env.irradiance_map = cubemap::create_irradiance(cubemap, 32);
			env.prefilter_map = cubemap::create_prefilter(cubemap, 128);
			env.brdf_lut_map = cubemap::create_brdf_lut_map(512);
		}

		{ // Directional Light
			entity entity = this->scene->create_entity();
			auto &dir_light = entity.add_component<directional_light_component>();
			dir_light.direction = glm::normalize(glm::vec3(1, -1, -1));
			dir_light.color = {1.0f, 0.79f, 0.56f};
			dir_light.intensity = 1.0f;
		}

		{ // Cube
			auto mat = std::make_shared<pbr_material>();
			mat->shader = pbr_shader;
			mat->albedo = texture_white;
			mat->normal = texture_white; // TODO: Use a proper normal texture
			mat->roughness = texture_white;
			mat->ao = texture_white;
			mat->metalness = texture_black;

			entity entity = this->scene->create_entity();

			auto &renderer = entity.add_component<mesh_renderer_component>();
			renderer.mesh = mesh::create("content/meshes/cube.obj");
			renderer.material = mat;

			entity.add_component<transform_component>(glm::mat4(1.0f));
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

		this->entity_properties_panel.on_imgui_render();
		this->scene_properties_panel.on_imgui_render(this->scene);
		this->scene_hierarchy_panel.on_imgui_render();
		this->shader_library_panel.on_imgui_render(this->shader_lib);
		this->viewport_panel.on_imgui_render();
	}
}
