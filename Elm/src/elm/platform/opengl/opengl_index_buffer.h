#pragma once

#include "elm/core/renderer/index_buffer.h"
#include <stdint.h>

namespace elm {

	class opengl_index_buffer : public index_buffer
	{
	public:
		opengl_index_buffer(const uint32_t *indices, uint32_t count);
		virtual ~opengl_index_buffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

		inline virtual uint32_t get_count(void) const override { return m_count; }

	private:
		uint32_t m_renderer_id;
		uint32_t m_count;
	};
}
