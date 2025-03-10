#include "opengl_uniform_buffer.h"
#include <glad/glad.h>

namespace elm {

	opengl_uniform_buffer::opengl_uniform_buffer(uint32_t size, uint32_t binding)
		: m_binding(binding)
	{
		glCreateBuffers(1, &m_renderer_id);
		glNamedBufferData(m_renderer_id, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_renderer_id);
	}

	opengl_uniform_buffer::~opengl_uniform_buffer(void)
	{
		glDeleteBuffers(1, &m_renderer_id);
	}

	void opengl_uniform_buffer::bind(void)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, m_renderer_id);
	}

	void opengl_uniform_buffer::unbind(void)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, 0);
	}

	void opengl_uniform_buffer::set_data(const void *data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_renderer_id, offset, size, data);
	}
}
