#pragma once

#include "elm/core/renderer/vertex_buffer.h"
#include <stdint.h>

namespace elm {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void *vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

	private:
		uint32_t m_renderer_id;

	};
}
