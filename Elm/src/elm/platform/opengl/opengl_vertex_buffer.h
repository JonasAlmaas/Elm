#pragma once

#include "elm/core/renderer/vertex_buffer.h"
#include <stdint.h>

namespace elm {

	class opengl_vertex_buffer : public vertex_buffer
	{
	public:
		opengl_vertex_buffer(const void *vertices, uint32_t size);
		virtual ~opengl_vertex_buffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

	private:
		uint32_t m_renderer_id;

	};
}
