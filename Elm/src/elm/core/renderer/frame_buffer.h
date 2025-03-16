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
		texture_wrap wrap_r = texture_wrap::CLAMP_TO_EDGE;
		texture_wrap wrap_s = texture_wrap::CLAMP_TO_EDGE;
		texture_wrap wrap_t = texture_wrap::CLAMP_TO_EDGE;
		texture_filter min_filter = texture_filter::LINEAR;
		texture_filter mag_filter = texture_filter::NEAREST;
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

	struct frame_buffer
	{
		virtual ~frame_buffer(void) = default;

		virtual void bind(void) = 0;
		virtual void unbind(void) = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual int read_px_int(uint32_t attachment_ix, int x, int y) const = 0;
		virtual void clear_attachment_int(uint32_t attachment_ix, int value) = 0;

		virtual uint32_t get_color_attachment_renderer_id(uint32_t ix) const = 0;
		virtual uint32_t get_depth_attachment_renderer_id(void) const = 0;

		virtual const struct frame_buffer_specification *get_spec(void) const = 0;

	public:
		static std::shared_ptr<frame_buffer> create(const struct frame_buffer_specification &spec);
	};
}
