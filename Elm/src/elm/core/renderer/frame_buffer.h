#pragma once

#include "elm/core/base.h"
#include "texture.h"
#include <memory>
#include <stdint.h>
#include <vector>
#include <initializer_list>

namespace elm {

	enum class frame_buffer_texture_format {
		None = 0,

		// Color
		RGBA8,
		RED_INT,

		// Depth / stencil
		DEPTH24STENCIL8,
	};

	struct frame_buffer_texture_specification {
		frame_buffer_texture_specification(void) = default;
		frame_buffer_texture_specification(frame_buffer_texture_format format)
			: texture_format(format) {}

		frame_buffer_texture_format texture_format = frame_buffer_texture_format::None;
		texture_2d_wrap wrap_r = texture_2d_wrap::CLAMP_TO_EDGE;
		texture_2d_wrap wrap_s = texture_2d_wrap::CLAMP_TO_EDGE;
		texture_2d_wrap wrap_t = texture_2d_wrap::CLAMP_TO_EDGE;
		texture_2d_filter min_filter = texture_2d_filter::LINEAR;
		texture_2d_filter mag_filter = texture_2d_filter::NEAREST;
	};

	struct frame_buffer_attachment_specification {
		frame_buffer_attachment_specification(void) = default;
		frame_buffer_attachment_specification(std::initializer_list<frame_buffer_texture_specification> attachments)
			: attachments(attachments) {}

		std::vector<frame_buffer_texture_specification> attachments;
	};

	struct frame_buffer_specification {
		uint32_t width;
		uint32_t height;
		frame_buffer_attachment_specification attachments;
		uint32_t samples = 1;
		/*bool is_swap_chain_target = false;*/
	};

	class frame_buffer
	{
	public:
		virtual ~frame_buffer(void) = default;

		virtual void bind(void) = 0;
		virtual void unbind(void) = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t get_color_attachment_renderer_id(uint32_t ix) const = 0;
		virtual uint32_t get_depth_attachment_renderer_id(void) const = 0;

		virtual const struct frame_buffer_specification *get_spec(void) const = 0;

	public:
		static std::shared_ptr<frame_buffer> create(const struct frame_buffer_specification &spec);
	};
}
