#include "opengl_vertex_buffer.h"

#include <glad/glad.h>

namespace elm {

	opengl_vertex_buffer::opengl_vertex_buffer(const void *vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
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
}
