#include "opengl_renderer_api.hpp"

#include <glad/glad.h>

namespace elm {

	void opengl_renderer_api::init()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void opengl_renderer_api::get_viewport(uint32_t *x, uint32_t *y, uint32_t *width, uint32_t *height)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);

		*x = viewport[0];
		*y = viewport[1];
		*width = viewport[2];
		*height = viewport[3];
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

	void opengl_renderer_api::clear()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void opengl_renderer_api::set_depth_test(bool enabled)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (enabled) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void opengl_renderer_api::set_depth_fn(depth_function fn)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		GLenum gl_func;

		switch (fn) {
		case depth_function::NEVER: gl_func = GL_NEVER; break;
		case depth_function::ALWAYS: gl_func = GL_ALWAYS; break;
		case depth_function::EQUAL: gl_func = GL_EQUAL; break;
		case depth_function::NOTEQUAL: gl_func = GL_NOTEQUAL; break;
		case depth_function::LESS: gl_func = GL_LESS; break;
		case depth_function::GREATER: gl_func = GL_GREATER; break;
		case depth_function::LESSEQUAL: gl_func = GL_LEQUAL; break;
		case depth_function::GREATEREQUAL: gl_func = GL_GEQUAL; break;
		default:
			ELM_CORE_ASSERT(false, "Unknwon depth function");
			gl_func = GL_LESS;
			break;
		}

		glDepthFunc(gl_func);
	}

	void opengl_renderer_api::draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t index_count)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		uint32_t count = index_count ? index_count : vertex_array->get_index_buffer()->get_count();
		ELM_CORE_ASSERT(count <= vertex_array->get_index_buffer()->get_count(), "Trying to draw more indicies than index buffer contains");
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void opengl_renderer_api::draw_triangles(uint32_t count, uint32_t offset)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDrawArrays(GL_TRIANGLES, offset, count);
	}

	void opengl_renderer_api::draw_line(const std::shared_ptr<vertex_array> &vertex_array, uint32_t vertex_count)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDrawArrays(GL_LINES, 0, vertex_count);
	}

	void opengl_renderer_api::set_line_thickness(float thickness)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glLineWidth(thickness);
	}

	std::string opengl_renderer_api::get_vendor()
	{
		return std::string((const char *)glGetString(GL_VENDOR));
	}

	std::string opengl_renderer_api::get_render()
	{
		return std::string((const char *)glGetString(GL_RENDERER));
	}

	std::string opengl_renderer_api::get_render_version()
	{
		return std::string((const char *)glGetString(GL_VERSION));
	}
}
