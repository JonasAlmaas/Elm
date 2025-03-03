#include "renderer.h"
#include "renderer_2d.h"
#include "render_command.h"

namespace elm {

	std::unique_ptr<renderer::scene_data> renderer::s_scene_data = std::make_unique<renderer::scene_data>(glm::mat4(1.0f));

	void renderer::init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::init();
		renderer_2d::init();
	}

	void renderer::on_window_resize(uint32_t width, uint32_t height)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		render_command::set_viewport(0, 0, width, height);
	}

	void renderer::begin_scene(const camera *camera)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		s_scene_data->view_projection_matrix = camera->get_view_projection_matrix();
	}

	void renderer::end_scene(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();
	}

	void renderer::submit(
		const std::shared_ptr<shader> &shader,
		const std::shared_ptr<vertex_array> &vertex_array,
		const glm::mat4 &transform)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		shader->bind();
		shader->set_mat4("u_view_projection", s_scene_data->view_projection_matrix);
		shader->set_mat4("u_transform", transform);

		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}
}
