#pragma once

#include "elm/core/renderer/index_buffer.hpp"
#include <stdint.h>

namespace elm {

	struct opengl_index_buffer : index_buffer
	{
		opengl_index_buffer(const uint32_t *indices, uint32_t count);
		virtual ~opengl_index_buffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

		inline virtual uint32_t get_count(void) const override { return this->count; }

	private:
		uint32_t renderer_id;
		uint32_t count;
	};
}
