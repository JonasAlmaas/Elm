#include "opengl_texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace elm {

	opengl_texture_2d::opengl_texture_2d(uint32_t width, uint32_t height)
		: m_width(width), m_height(height), m_fpath("<BUFFER>")
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		m_internal_format = GL_RGBA8;
		m_data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	opengl_texture_2d::opengl_texture_2d(const std::string &fpath)
		: m_fpath(fpath)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc *data = stbi_load(fpath.c_str(), &width, &height, &channels, 0);
		ELM_CORE_ASSERT(data, "Failed to load texture");

		m_width = (uint32_t)width;
		m_height = (uint32_t)height;

		m_internal_format = 0;
		m_data_format = 0;

		switch (channels) {
		case 3:
			m_internal_format = GL_RGB8;
			m_data_format = GL_RGB;
			break;
		case 4:
			m_internal_format = GL_RGBA8;
			m_data_format = GL_RGBA;
			break;
		default:
			ELM_CORE_ASSERT(false, "Texture format not supported");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, (const void *)data);

		stbi_image_free(data);
	}

	opengl_texture_2d::~opengl_texture_2d(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteTextures(1, &m_renderer_id);
	}

	void opengl_texture_2d::bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

	void opengl_texture_2d::set_data(void *data, uint32_t size)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		uint32_t channel_size = m_data_format == GL_RGBA ? 4 : 3;
		ELM_ASSERT(size == m_width * m_height * channel_size, "Data must match texture size");

		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, (const void *)data);
	}
}
