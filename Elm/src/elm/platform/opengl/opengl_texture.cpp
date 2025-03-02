#include "opengl_texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace elm {

	opengl_texture_2d::opengl_texture_2d(const std::string &fpath)
		: m_fpath(fpath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc *data = stbi_load(fpath.c_str(), &width, &height, &channels, 0);
		ELM_CORE_ASSERT(data, "Failed to load texture");

		m_width = (uint32_t)width;
		m_height = (uint32_t)height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, GL_RGB8, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, (const void *)data);

		stbi_image_free(data);
	}

	opengl_texture_2d::~opengl_texture_2d(void)
	{
		glDeleteTextures(1, &m_renderer_id);
	}

	void opengl_texture_2d::bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_renderer_id);
	}
}
