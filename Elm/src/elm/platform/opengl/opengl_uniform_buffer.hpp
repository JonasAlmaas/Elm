#pragma once

#include "elm/core/renderer/uniform_buffer.hpp"

namespace elm {

	struct opengl_uniform_buffer : uniform_buffer
	{
		opengl_uniform_buffer(uint32_t size, uint32_t binding);
		virtual ~opengl_uniform_buffer(void);

		virtual void bind(void) override;
		virtual void unbind(void) override;

		virtual void set_data(const void *data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t renderer_id;
		uint32_t binding;
	};
}
