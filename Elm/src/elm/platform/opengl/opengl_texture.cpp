#include "opengl_texture.h"

#include "opengl_utils.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace elm {

	static GLenum image_format_to_gl(image_format format)
	{
		switch (format) {
		case elm::image_format::R8:
			return GL_R;
		case elm::image_format::RGB8:
		case elm::image_format::RGB16F:
		case elm::image_format::RGB32F:
			return GL_RGB;
		case elm::image_format::RGBA8:
		case elm::image_format::RGBA16F:
		case elm::image_format::RGBA32F:
			return GL_RGBA;
		default:
			ELM_CORE_ASSERT(false, "Unknown image format");
			return 0;
		}
	}

	static GLenum image_format_to_gl_internal(image_format format)
	{
		switch (format) {
		case elm::image_format::R8: return GL_R8;
		case elm::image_format::RGB8: return GL_RGB8;
		case elm::image_format::RGB16F: return GL_RGB16F;
		case elm::image_format::RGB32F: return GL_RGB32F;
		case elm::image_format::RGBA8: return GL_RGBA8;
		case elm::image_format::RGBA16F: return GL_RGBA16F;
		case elm::image_format::RGBA32F: return GL_RGBA32F;
		default:
			ELM_CORE_ASSERT(false, "Unknown image format");
			return 0;
		}
	}

	static GLenum image_format_to_gl_type(image_format format)
	{
		switch (format) {
		case elm::image_format::R8:
		case elm::image_format::RGB8:
		case elm::image_format::RGBA8:
			return GL_BYTE;
		case elm::image_format::RGB16F:
		case elm::image_format::RGB32F:
		case elm::image_format::RGBA16F:
		case elm::image_format::RGBA32F:
			return GL_FLOAT;
		default:
			ELM_CORE_ASSERT(false, "Unknown image format");
			return 0;
		}
	}

	opengl_texture_2d::opengl_texture_2d(const texture_specification &spec)
		: m_width(spec.width), m_height(spec.height), m_spec(spec), m_fpath("<BUFFER>")
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		m_internal_format = image_format_to_gl_internal(spec.format);
		m_data_format = image_format_to_gl(spec.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, utils::texture_2d_filtering_to_gl(m_spec.min_filter));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, utils::texture_2d_filtering_to_gl(m_spec.mag_filter));

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, utils::texture_2d_wrap_to_gl(m_spec.wrap_s));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, utils::texture_2d_wrap_to_gl(m_spec.wrap_t));

		if (m_spec.generate_mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	opengl_texture_2d::opengl_texture_2d(const std::string &fpath, const texture_specification &spec)
		: m_fpath(fpath), m_spec(spec)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc *data = stbi_load(fpath.c_str(), &width, &height, &channels, 0);
		ELM_CORE_ASSERT(data, "Failed to load texture");

		m_width = (uint32_t)width;
		m_height = (uint32_t)height;

		m_internal_format = image_format_to_gl_internal(spec.format);
		m_data_format = image_format_to_gl(spec.format);

		ELM_CORE_ASSERT(
			(channels == 1 && m_data_format == GL_R)
			|| (channels == 3 && m_data_format == GL_RGB)
			|| (channels == 4 && m_data_format == GL_RGBA),
			"Channel count does not match texture format");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, utils::texture_2d_filtering_to_gl(m_spec.min_filter));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, utils::texture_2d_filtering_to_gl(m_spec.mag_filter));

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, utils::texture_2d_wrap_to_gl(m_spec.wrap_s));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, utils::texture_2d_wrap_to_gl(m_spec.wrap_t));

		if (m_spec.generate_mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}

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

	// -----------------------------------------------------
	// -- Texture cube -------------------------------------
	// -----------------------------------------------------

	opengl_texture_cube::opengl_texture_cube(const texture_specification &spec)
		: m_width(spec.width), m_height(spec.height), m_spec(spec)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		m_internal_format = image_format_to_gl_internal(spec.format);
		m_data_format = image_format_to_gl(spec.format);
		auto type = image_format_to_gl_type(spec.format);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_renderer_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_id);

		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_internal_format, m_width, m_height, 0, m_data_format, type, nullptr);
		}

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, utils::texture_2d_filtering_to_gl(m_spec.min_filter));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, utils::texture_2d_filtering_to_gl(m_spec.mag_filter));

		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, utils::texture_2d_wrap_to_gl(m_spec.wrap_s));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, utils::texture_2d_wrap_to_gl(m_spec.wrap_t));
		glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_R, utils::texture_2d_wrap_to_gl(m_spec.wrap_r));

		if (m_spec.generate_mipmaps) {
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}

	opengl_texture_cube::~opengl_texture_cube(void)
	{
		ELM_PROFILE_RENDERER_FUNCTION();

		glDeleteTextures(1, &m_renderer_id);
	}

	void opengl_texture_cube::bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

	void opengl_texture_cube::set_data(void *data, uint32_t size)
	{
		// TODO: Implement
		ELM_CORE_ASSERT(false, "Not implemented");
	}
}
