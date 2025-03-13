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
		float intensity;
		glm::vec3 color;
		float ambient_intensity;
		glm::vec3 ambient_color;
	};

	struct lights_data {
		struct directional_light dir_light;
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
		ELM_PROFILE_RENDERER_SCOPE("render_mesh_render_components()");

		auto view = reg.view<transform_component, mesh_renderer_component>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, mesh_renderer_component>(entity);
			for (uint32_t i = 0; i < rc.textures.size(); ++i) {
				rc.textures[i]->bind(i);
			}
			renderer::submit(rc.shader, rc.mesh->vertex_array, tc.transform);
		}
	}

	inline static void render_circle_renderer_components(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_SCOPE("render_circle_renderer_components()");

		auto view = reg.view<transform_component, circle_renderer_component>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, circle_renderer_component>(entity);
			renderer_2d::draw_circle(tc.transform, rc.color, rc.radius, rc.thickness, rc.fade);
		}
	}

	inline static void prepare_lights(const entt::registry &reg)
	{
		ELM_PROFILE_RENDERER_SCOPE("prepare_lights()");

		static struct lights_data s_lights_data;

		auto view = reg.view<directional_light_component>();
		ELM_CORE_ASSERT(view.size(), "Scene must have one directional light");
		for (auto entity : view) {
			auto &dlc = view.get<directional_light_component>(entity);

			s_lights_data.dir_light.direction = dlc.direction;
			s_lights_data.dir_light.color = dlc.color;
			s_lights_data.dir_light.intensity = dlc.intensity;
			s_lights_data.dir_light.ambient_color = dlc.ambient_color;
			s_lights_data.dir_light.ambient_intensity = dlc.ambient_intensity;
		}

		s_data.lights_ub->bind();
		s_data.lights_ub->set_data((const void *)&s_lights_data, sizeof s_lights_data);
	}

	inline static void render_world_grid(const camera *camera)
	{
		ELM_PROFILE_RENDERER_SCOPE("render_world_grid()");

		// TODO: Configure world grid data

		s_data.world_grid_shader->bind();
		elm::render_command::draw_arrays(6);
	}

	extern void render(std::shared_ptr<scene> scene, const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto &reg = scene->get_reg();

		render_command::set_clear_color(scene->get_clear_color());
		render_command::clear();

		renderer_2d::begin_scene(camera);
		render_circle_renderer_components(reg);
		renderer_2d::end_scene();

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
