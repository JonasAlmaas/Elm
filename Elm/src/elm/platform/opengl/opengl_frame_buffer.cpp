#include "opengl_frame_buffer.h"

#include <glad/glad.h>

namespace elm {

	static const uint32_t s_max_frame_buffer_size = 16384u;

	namespace utils {

		static GLenum texture_traget(bool is_multisampled)
		{
			return is_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void create_textures(bool is_multisampled, uint32_t *out_id, uint32_t count)
		{
			glCreateTextures(texture_traget(is_multisampled), count, out_id);
		}

		static void bind_texture(bool is_multisampled, uint32_t id)
		{
			glBindTexture(texture_traget(is_multisampled), id);
		}

		static void attach_color_texture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool is_multisampled = samples > 1;

			if (is_multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_traget(is_multisampled), id, 0);
		}

		static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height)
		{
			bool is_multisampled = samples > 1;

			if (is_multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			} else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, texture_traget(is_multisampled), id, 0);
		}

		static bool is_depth_format(frame_buffer_texture_format foramt)
		{
			switch (foramt) {
			case elm::frame_buffer_texture_format::RGBA8:
			case elm::frame_buffer_texture_format::RED_INTEGER:
				return false;
			case elm::frame_buffer_texture_format::DEPTH24STENCIL8:
				return true;
			default:
				ELM_CORE_ASSERT(false, "Unknown frame buffer texture format");
				return false;
			}
		}

		static GLenum frame_buffer_texture_format_to_gl(frame_buffer_texture_format foramt)
		{
			switch (foramt) {
			case elm::frame_buffer_texture_format::RGBA8: return GL_RGBA8;
			case elm::frame_buffer_texture_format::RED_INTEGER: return GL_RED_INTEGER;
			default:
				ELM_CORE_ASSERT(false, "Unknown frame buffer texture format");
				return 0;
			}
		}
	}

	opengl_frame_buffer::opengl_frame_buffer(const struct frame_buffer_specification &spec)
		: m_spec(spec)
	{
		ELM_CORE_ASSERT(spec.width, "Framebuffer can not be created with a width of 0");
		ELM_CORE_ASSERT(spec.height, "Framebuffer can not be created with a height of 0");

		for (auto &spec : m_spec.attachments.attachments) {
			if (!utils::is_depth_format(spec.texture_format)) {
				m_color_attachment_specs.emplace_back(spec);
			} else {
				m_depth_attachment_spec = spec;
			}
		}

		invalidate();
	}

	opengl_frame_buffer::~opengl_frame_buffer(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteFramebuffers(1, &m_renderer_id);
		glDeleteTextures((GLsizei)m_color_attachments.size(), m_color_attachments.data());
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
			glDeleteTextures((GLsizei)m_color_attachments.size(), m_color_attachments.data());
			glDeleteTextures(1, &m_depth_attachment);

			m_color_attachments.clear();
			m_depth_attachment = 0;
		}

		glCreateFramebuffers(1, &m_renderer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

		bool is_multisampled = m_spec.samples > 1;

		// Setup color attachments

		if (m_color_attachment_specs.size()) {
			m_color_attachments.resize(m_color_attachment_specs.size());
			utils::create_textures(is_multisampled, m_color_attachments.data(), (uint32_t)m_color_attachments.size());

			for (size_t i = 0; i < m_color_attachment_specs.size(); ++i) {
				utils::bind_texture(is_multisampled, m_color_attachments[i]);

				switch (m_color_attachment_specs[i].texture_format) {
				case frame_buffer_texture_format::RGBA8: utils::attach_color_texture(m_color_attachments[i], m_spec.samples, GL_RGBA8, GL_RGBA, m_spec.width, m_spec.height, (int)i); break;
				case frame_buffer_texture_format::RED_INTEGER: utils::attach_color_texture(m_color_attachments[i], m_spec.samples, GL_R32I, GL_RED_INTEGER, m_spec.width, m_spec.height, (int)i); break;
				default:
					ELM_CORE_ASSERT(false, "Unknown frame buffer attachment texture format");
					break;
				}
			}
		}

		// Setup depth attachment

		if (m_depth_attachment_spec.texture_format != frame_buffer_texture_format::None) {
			utils::create_textures(is_multisampled, &m_depth_attachment, 1);
			utils::bind_texture(is_multisampled, m_depth_attachment);

			switch (m_depth_attachment_spec.texture_format) {
			case frame_buffer_texture_format::DEPTH24STENCIL8: utils::attach_depth_texture(m_depth_attachment, m_spec.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_spec.width, m_spec.height); break;
			default:
				ELM_CORE_ASSERT(false, "Unknown frame buffer attachment texture format");
				break;
			}
		}

		// Setup rendering to multiple color attachments

		if (m_color_attachments.size() > 1) {
			// Just up the amout if needed
			ELM_CORE_ASSERT(m_color_attachments.size() <= 4, "More color attachments specified than what is support");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_color_attachments.size(), buffers);
		} else if (m_color_attachments.empty()) { // Only depth pass
			glDrawBuffer(GL_NONE);
		}

		ELM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
