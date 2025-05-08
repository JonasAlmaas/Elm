#pragma once

#include "elm/core/renderer/uniform_buffer.hpp"

namespace elm {

	struct opengl_uniform_buffer final : uniform_buffer
	{
		opengl_uniform_buffer(uint32_t size, uint32_t binding);
		virtual ~opengl_uniform_buffer();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void set_data(const void *data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t renderer_id;
		uint32_t binding;
	};
}
