#include "opengl_texture.h"

#include "opengl_utils.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace elm {

	static GLenum image_format_to_gl(image_format format)
	{
		switch (format) {
		//case elm::image_format::R8: return
		case elm::image_format::RGB8: return GL_RGB;
		case elm::image_format::RGBA8: return GL_RGBA;
		//case elm::image_format::RGBA32F: return
		default:
			ELM_CORE_ASSERT(false, "Unknown image format");
			return 0;
		}
	}

	static GLenum image_format_to_gl_internal(image_format format)
	{
		switch (format) {
		//case elm::image_format::R8: return
		case elm::image_format::RGB8: return GL_RGB8;
		case elm::image_format::RGBA8: return GL_RGBA8;
		//case elm::image_format::RGBA32F: return
		default:
			ELM_CORE_ASSERT(false, "Unknown image format");
			return 0;
		}
	}

	opengl_texture_2d::opengl_texture_2d(uint32_t width, uint32_t height, texture_2d_specification spec)
		: m_width(width), m_height(height), m_spec(spec), m_fpath("<BUFFER>")
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		// TODO: Parameterize this
		m_internal_format = image_format_to_gl_internal(spec.format);
		m_data_format = image_format_to_gl(spec.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, utils::texture_2d_filtering_to_gl(m_spec.min_filter));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, utils::texture_2d_filtering_to_gl(m_spec.mag_filter));

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, utils::texture_2d_wrap_to_gl(m_spec.wrap_s));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, utils::texture_2d_wrap_to_gl(m_spec.wrap_t));
	}

	opengl_texture_2d::opengl_texture_2d(const std::string &fpath, texture_2d_specification spec)
		: m_fpath(fpath), m_spec(spec)
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

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, utils::texture_2d_filtering_to_gl(m_spec.min_filter));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, utils::texture_2d_filtering_to_gl(m_spec.mag_filter));

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, utils::texture_2d_wrap_to_gl(m_spec.wrap_s));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, utils::texture_2d_wrap_to_gl(m_spec.wrap_t));

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
