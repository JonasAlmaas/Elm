#include "renderer.h"
#include "renderer_2d.h"
#include "elm/core/renderer/render_command.h"

namespace elm::renderer {

	struct scene_data {
		glm::mat4 view_projection_matrix;
	};

	static struct scene_data s_scene_data = {
		.view_projection_matrix = glm::mat4(1.0f)
	};

	extern void init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::init();
		renderer_2d::init();
	}

	extern void on_viewport_resize(uint32_t width, uint32_t height)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::set_viewport(0, 0, width, height);
	}

	extern void begin_scene(const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_scene_data.view_projection_matrix = camera->get_view_projection_matrix();
	}

	extern void end_scene(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();
	}

	extern void submit(
		const std::shared_ptr<shader> &shader,
		const std::shared_ptr<vertex_array> &vertex_array,
		const glm::mat4 &transform)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		shader->bind();
		shader->set_mat4("u_view_projection", s_scene_data.view_projection_matrix);
		shader->set_mat4("u_transform", transform);

		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}
}
