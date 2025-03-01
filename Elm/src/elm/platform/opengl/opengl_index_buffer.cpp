#include "opengl_index_buffer.h"

#include <glad/glad.h>

namespace elm {

	opengl_index_buffer::opengl_index_buffer(const uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		glCreateBuffers(1, &m_renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void opengl_index_buffer::bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
	}

	void opengl_index_buffer::unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
