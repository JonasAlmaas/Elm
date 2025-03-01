#include "renderer.h"
#include "render_command.h"

namespace elm {

	std::unique_ptr<renderer::scene_data> renderer::s_scene_data = std::make_unique<renderer::scene_data>(glm::mat4(1.0f));

	void renderer::begin_scene(const camera *camera)
	{
		s_scene_data->view_projection_matrix = camera->get_view_projection_matrix();
	}

	void renderer::end_scene(void)
	{

	}

	void renderer::submit(const std::shared_ptr<shader> &shader, const std::shared_ptr<vertex_array> &vertex_array)
	{
		shader->bind();
		shader->upload_uniform_mat4("u_view_projection", s_scene_data->view_projection_matrix);
		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}
}
