#include "opengl_vertex_buffer.h"

#include <glad/glad.h>

namespace elm {

	opengl_vertex_buffer::opengl_vertex_buffer(uint32_t size)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	opengl_vertex_buffer::opengl_vertex_buffer(const void *vertices, uint32_t size)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &m_renderer_id);
	}

	void opengl_vertex_buffer::bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void opengl_vertex_buffer::unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void opengl_vertex_buffer::set_data(const void *data, uint32_t size)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void opengl_vertex_buffer::set_layout(const vertex_buffer_layout *layout)
	{
		m_layout = *layout;
	}
}
