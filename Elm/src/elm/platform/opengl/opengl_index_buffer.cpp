#include "opengl_index_buffer.hpp"

#include <glad/glad.h>

namespace elm {

	opengl_index_buffer::opengl_index_buffer(const uint32_t* indices, uint32_t count)
		: count(count)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glCreateBuffers(1, &this->renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteBuffers(1, &this->renderer_id);
	}

	void opengl_index_buffer::bind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->renderer_id);
	}

	void opengl_index_buffer::unbind(void) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
