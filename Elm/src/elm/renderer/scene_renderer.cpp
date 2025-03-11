#include "scene_renderer.h"
#include "elm/core/renderer/render_command.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/uniform_buffer.h"
#include "elm/math/math.h"
#include "elm/renderer/renderer.h"
#include "elm/renderer/renderer_2d.h"
#include "elm/scene/components.h"
#include <glm/glm.hpp>

namespace elm::scene_renderer {

	struct scene_renderer_data {
		std::shared_ptr<elm::shader> world_grid_shader;
		std::shared_ptr<elm::uniform_buffer> world_grid_ub;
		struct world_grid_data {
			glm::vec3 camera_position;
		};
		struct world_grid_data world_grid_data;
	};

	static struct scene_renderer_data s_data;

	void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_data.world_grid_shader = elm::shader::create("content/shaders/world_grid.glsl");
		s_data.world_grid_ub = elm::uniform_buffer::create(sizeof s_data.world_grid_data, 1);
	}

	void shutdown(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();
	}

	extern void render(std::shared_ptr<scene> scene, const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		auto &reg = scene->get_reg();

		render_command::set_clear_color(scene->get_clear_color());
		render_command::clear();

		renderer::begin_scene(camera);

		auto view = reg.view<transform_component, quick_and_dirty_mesh_renderer>();
		for (auto entity : view) {
			auto [tc, rc] = view.get<transform_component, quick_and_dirty_mesh_renderer>(entity);
			rc.texture->bind();
			renderer::submit(rc.shader, rc.vertex_array, tc.transform);
		}

		renderer_2d::begin_scene(camera, false);
		{
			ELM_PROFILE_RENDERER_SCOPE("scene_renderer::render() - 2D");

			auto view = reg.view<transform_component, circle_renderer_component>();
			for (auto entity : view) {
				auto [tc, rc] = view.get<transform_component, circle_renderer_component>(entity);
				renderer_2d::draw_circle(tc.transform, rc.color, rc.radius, rc.thickness, rc.fade);
			}

			renderer_2d::end_scene();
		}

		// Render world grid if enabled
		if (scene->get_show_world_grid()) {
			ELM_PROFILE_RENDERER_SCOPE("scene_renderer::render() - World grid");
			// TODO: Configure world grid data

			math::decompose_transform(
				glm::inverse(camera->get_view()),
				&s_data.world_grid_data.camera_position,
				nullptr,
				nullptr);
			s_data.world_grid_shader->bind();
			s_data.world_grid_ub->bind();
			s_data.world_grid_ub->set_data((const void *)&s_data.world_grid_data, sizeof s_data.world_grid_data);
			elm::render_command::draw_arrays(6);
		}

		renderer::end_scene();
	}
}
