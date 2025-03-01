#pragma once

#include "elm/core/renderer/index_buffer.h"
#include <stdint.h>

namespace elm {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t *indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

		inline virtual uint32_t get_count(void) const override { return m_count; }

	private:
		uint32_t m_renderer_id;
		uint32_t m_count;

	};
}
