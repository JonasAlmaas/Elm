#include "scene_renderer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/uniform_buffer.h"
#include "elm/math/math.h"
#include "elm/renderer/renderer.h"
#include "elm/renderer/renderer_2d.h"
#include "elm/scene/components.h"
#include <glm/glm.hpp>

// Temp
#include <entt/entt.hpp>

namespace elm::scene_renderer {

	struct scene_renderer_data {
		std::shared_ptr<elm::uniform_buffer> lights_ub;
		std::shared_ptr<elm::shader> world_grid_shader;
	};

	struct directional_light { // Strange order so it aligns in gpu memory. vec3 has an alignment of 16 byts
		glm::vec3 direction;
		float padding;
		glm::vec3 color;
		float padding2;
	};

	struct point_light {
		glm::vec3 position;
		float padding;
		glm::vec3 color;
		float padding2;
	};

	enum {MAX_POINT_LIGHTS=4};

	struct lights_data {
		struct directional_light dir_light;
		int point_light_count;
		uint32_t padding2[3];
		struct point_light point_lights[MAX_POINT_LIGHTS];
	};

	static struct scene_renderer_data s_data;

	void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.lights_ub = elm::uniform_buffer::create(sizeof(struct lights_data), 2);

		s_data.world_grid_shader = elm::shader::create("content/shaders/world_grid.glsl");
	}

	void shutdown(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();
	}

	inline static void render_mesh_render_components(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto view = reg.view<transform_component, mesh_renderer_component>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, mesh_renderer_component>(entity);
			rc.material->bind();
			renderer::submit(rc.material->shader, rc.mesh->vertex_array, tc.transform);
		}
	}

	inline static void render_sprite_renderer_components(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto view = reg.view<transform_component, sprite_renderer_component>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, sprite_renderer_component>(entity);
			if (rc.texture) {
				renderer_2d::draw_sprite(tc.transform, rc.texture, rc.tiling_factor, rc.color);
			} else {
				renderer_2d::draw_sprite(tc.transform, rc.color);
			}
		}
	}

	inline static void render_circle_renderer_components(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto view = reg.view<transform_component, circle_renderer_component>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, circle_renderer_component>(entity);
			renderer_2d::draw_circle(tc.transform, rc.color, rc.radius, rc.thickness, rc.fade);
		}
	}

	inline static void render_text_renderer_components(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto view = reg.view<transform_component, text_renderer_component>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, text_renderer_component>(entity);
			renderer_2d::text_render_params params = {
				.color = rc.color,
				.kerning = rc.kerning,
				.line_spacing = rc.line_spacing};
			renderer_2d::draw_text(rc.text, rc.font, tc.transform, params);
		}
	}

	inline static void prepare_lights(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		static struct lights_data s_lights_data;

		{ // Environment light
			auto view = reg.view<environment_light_component>();
			ELM_CORE_ASSERT(view.size(), "Scene must have one environment light");
			for (auto entity : view) {
				auto& elc = view.get<environment_light_component>(entity);

				// TODO: Set this per draw maybe?
				// Kinda assuming we are only using one shader...
				elc.irradiance_map->bind(0);
				elc.prefilter_map->bind(1);
				elc.brdf_lut_map->bind(2);
			}
		}

		{ // Directional light
			auto view = reg.view<directional_light_component>();
			ELM_CORE_ASSERT(view.size(), "Scene must have one directional light");
			for (auto entity : view) {
				auto &dlc = view.get<directional_light_component>(entity);

				s_lights_data.dir_light.direction = dlc.direction;
				s_lights_data.dir_light.color = dlc.color * dlc.intensity;
			}
		}

		{ // Point lights
			s_lights_data.point_light_count = 0;

			auto view = reg.view<transform_component, point_light_component>();
			for (auto entity : view) {
				auto [tc, plc] = view.get<transform_component, point_light_component>(entity);

				if (s_lights_data.point_light_count < MAX_POINT_LIGHTS) {
					auto &pl = s_lights_data.point_lights[s_lights_data.point_light_count];

					elm::math::decompose_transform(tc.transform, &pl.position, nullptr, nullptr);
					pl.color = plc.color * plc.intensity;
					
					++s_lights_data.point_light_count;
				} else {
					ELM_CORE_WARN("Only {0} point lights are currently supported", (int)MAX_POINT_LIGHTS);
				}
			}
		}

		s_data.lights_ub->bind();
		s_data.lights_ub->set_data((const void *)&s_lights_data, sizeof s_lights_data);
	}

	inline static void render_world_grid(const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		// TODO: Configure world grid data

		s_data.world_grid_shader->bind();
		elm::render_command::draw_triangles(6);
	}

	extern void render(std::shared_ptr<scene> scene, const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto &reg = scene->get_reg();

		render_command::set_clear_color(scene->get_clear_color());
		render_command::clear();

		render_command::set_depth_test(true);
		render_command::set_depth_fn(depth_function::Less);

		// -- Renderer 2d --
		renderer_2d::begin_scene(camera);

		render_sprite_renderer_components(reg);
		render_circle_renderer_components(reg);
		render_text_renderer_components(reg);

		renderer_2d::end_scene();
		// -- Renderer 3d --
		renderer::begin_scene(camera);

		prepare_lights(reg);

		render_mesh_render_components(reg);

		// Render world grid if enabled
		if (scene->get_show_world_grid()) {
			render_world_grid(camera);
		}

		renderer::end_scene();
	}
}
