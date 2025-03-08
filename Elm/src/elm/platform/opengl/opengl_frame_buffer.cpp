#include "opengl_frame_buffer.h"

#include <glad/glad.h>

namespace elm {

	static const uint32_t s_max_frame_buffer_size = 16384u;

	opengl_frame_buffer::opengl_frame_buffer(const struct frame_buffer_specification &spec)
		: m_spec(spec)
	{
		invalidate();
	}

	opengl_frame_buffer::~opengl_frame_buffer(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteFramebuffers(1, &m_renderer_id);
		glDeleteTextures(1, &m_color_attachment);
		glDeleteTextures(1, &m_depth_attachment);
	}

	void opengl_frame_buffer::bind(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
		glViewport(0, 0, m_spec.width, m_spec.height);
	}

	void opengl_frame_buffer::unbind(void)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void opengl_frame_buffer::resize(uint32_t width, uint32_t height)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (width == 0 || height == 0 || width > s_max_frame_buffer_size || height > s_max_frame_buffer_size) {
			ELM_CORE_WARN("Atempted to resize frame buffer to {} {}", width, height);
			return;
		}

		m_spec.width = width;
		m_spec.height = height;

		invalidate();
	}

	void opengl_frame_buffer::invalidate(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		if (m_renderer_id) {
			glDeleteFramebuffers(1, &m_renderer_id);
			glDeleteTextures(1, &m_color_attachment);
			glDeleteTextures(1, &m_depth_attachment);
		}

		glCreateFramebuffers(1, &m_renderer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

		// Color attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment);
		glBindTexture(GL_TEXTURE_2D, m_color_attachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.width, m_spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);

		// Depth attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
		glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

		ELM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
