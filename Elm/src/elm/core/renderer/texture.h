#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>
#include <array>
#include <memory>
#include <stdint.h>
#include <string>

namespace elm {

	class texture
	{
	public:
		virtual ~texture(void) = default;

		virtual void bind(uint32_t slot = 0) = 0;

		virtual uint32_t get_width(void) const = 0;
		virtual uint32_t get_height(void) const = 0;

		virtual uint32_t get_renderer_id(void) const = 0;

		virtual void set_data(void *data, uint32_t size) = 0;

		virtual bool equal(const texture *other) const = 0;
	};

	enum class texture_2d_wrap {
		CLAMP,
		CLAMP_TO_EDGE,
		REPEAT,
	};

	enum class texture_2d_filter {
		NEAREST,
		LINEAR,
	};

	struct texture_2d_specification {
		texture_2d_wrap wrap_s = texture_2d_wrap::REPEAT;
		texture_2d_wrap wrap_t = texture_2d_wrap::REPEAT;
		texture_2d_filter min_filter = texture_2d_filter::LINEAR;
		texture_2d_filter mag_filter = texture_2d_filter::NEAREST;
	};

	class texture_2d : public texture
	{
	public:
		virtual ~texture_2d(void) = default;

	public:
		static std::shared_ptr<texture_2d> create(uint32_t width, uint32_t height, texture_2d_specification spec = texture_2d_specification());
		static std::shared_ptr<texture_2d> create(const std::string &fpath, texture_2d_specification spec = texture_2d_specification());
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
