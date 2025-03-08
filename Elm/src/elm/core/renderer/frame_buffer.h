#pragma once

#include "elm/core/base.h"
#include <memory>
#include <stdint.h>

namespace elm {

	struct frame_buffer_specification {
		uint32_t width;
		uint32_t height;
		//frame_buffer_format format;
		/*uint32_t samples = 1;

		bool is_swap_chain_target = false;*/
	};

	class frame_buffer
	{
	public:
		virtual ~frame_buffer(void) = default;

		virtual void bind(void) = 0;
		virtual void unbind(void) = 0;

		virtual uint32_t get_color_attachment_renderer_id(void) const = 0;

		virtual const struct frame_buffer_specification *get_spec(void) const = 0;

	public:
		static std::shared_ptr<frame_buffer> create(const struct frame_buffer_specification &spec);
	};
}
