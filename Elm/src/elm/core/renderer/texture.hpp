#pragma once

#include "elm/core/base.hpp"
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include <stdint.h>
#include <string>

namespace elm {

	enum class image_format {
		NONE = 0,
		R8,
		RGB8,
		RGB16F,
		RGB32F,
		RGBA8,
		RGBA16F,
		RGBA32F,
	};

	enum class texture_wrap {
		CLAMP,
		CLAMP_TO_EDGE,
		REPEAT,
	};

	enum class texture_filter {
		NEAREST,
		LINEAR,
		LINEAR_MIPMAP_LINEAR,
	};

	struct texture_specification {
		uint32_t width = 1u;
		uint32_t height = 1u;
		image_format format = image_format::RGBA8;
		texture_wrap wrap_s = texture_wrap::REPEAT;
		texture_wrap wrap_t = texture_wrap::REPEAT;
		texture_wrap wrap_r = texture_wrap::REPEAT;
		texture_filter min_filter = texture_filter::LINEAR;
		texture_filter mag_filter = texture_filter::LINEAR;
		bool generate_mipmaps = false;
	};

	struct texture
	{
		virtual ~texture() = default;

		virtual void bind(uint32_t slot = 0) = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		virtual uint32_t get_renderer_id() const = 0;

		virtual void set_data(void *data, uint32_t size) = 0;

		virtual const texture_specification &get_spec() = 0;

		virtual bool equal(const texture *other) const = 0;
	};

	struct texture_2d : texture
	{
		virtual ~texture_2d() = default;

	public:
		static std::shared_ptr<texture_2d> create(const texture_specification &spec = texture_specification());
		static std::shared_ptr<texture_2d> create(const std::string &fpath, const texture_specification &spec = texture_specification());
	};

	struct texture_cube : texture
	{
		virtual ~texture_cube() = default;

	public:
		static std::shared_ptr<texture_cube> create(const texture_specification &spec = texture_specification());
	};

	struct sub_texture_2d
	{
		std::shared_ptr<texture_2d> texture;
		std::array<glm::vec2, 4> uvs;

	public:
		static std::shared_ptr<sub_texture_2d> from_atlas(
			const std::shared_ptr<texture_2d> &texture,
			uint32_t sprite_width,
			uint32_t sprite_height,
			uint32_t x,
			uint32_t y);
	};
}
