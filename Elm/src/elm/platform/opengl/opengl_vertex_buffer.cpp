#include "opengl_vertex_buffer.h"

#include <glad/glad.h>

namespace elm {

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void *vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLVertexBuffer::bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLVertexBuffer::unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
