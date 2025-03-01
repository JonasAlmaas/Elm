#include "opengl_index_buffer.h"

#include <glad/glad.h>

namespace elm {

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void OpenGLIndexBuffer::bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void OpenGLIndexBuffer::unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
