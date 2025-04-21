#include "opengl_uniform_buffer.hpp"
#include <glad/glad.h>

namespace elm {

	opengl_uniform_buffer::opengl_uniform_buffer(uint32_t size, uint32_t binding)
		: binding(binding)
	{
		glCreateBuffers(1, &this->renderer_id);
		glNamedBufferData(this->renderer_id, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, this->binding, this->renderer_id);
	}

	opengl_uniform_buffer::~opengl_uniform_buffer()
	{
		glDeleteBuffers(1, &this->renderer_id);
	}

	void opengl_uniform_buffer::bind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, this->binding, this->renderer_id);
	}

	void opengl_uniform_buffer::unbind()
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, this->binding, 0);
	}

	void opengl_uniform_buffer::set_data(const void *data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(this->renderer_id, offset, size, data);
	}
}
