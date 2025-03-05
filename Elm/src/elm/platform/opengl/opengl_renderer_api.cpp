#include "opengl_renderer_api.h"

#include <glad/glad.h>

namespace elm {

	void opengl_renderer_api::init(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void opengl_renderer_api::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glViewport(x, y, width, height);
	}

	void opengl_renderer_api::set_clear_color(const glm::vec4 &color)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void opengl_renderer_api::clear(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void opengl_renderer_api::draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t count)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		uint32_t index_count = count == 0 ? vertex_array->get_index_buffer()->get_count() : count;
		ELM_CORE_ASSERT(index_count <= vertex_array->get_index_buffer()->get_count(), "Trying to draw more indicies than index buffer contains");
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
	}
}
