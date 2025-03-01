#pragma once

#include "elm/core/renderer/vertex_buffer.h"
#include <stdint.h>
#include <memory>

namespace elm {

	class opengl_vertex_buffer : public vertex_buffer
	{
	public:
		opengl_vertex_buffer(const void *vertices, uint32_t size);
		virtual ~opengl_vertex_buffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

		inline virtual const vertex_buffer_layout *get_layout(void) const override { return &m_layout; }
		virtual void set_layout(const vertex_buffer_layout *layout) override;

	private:
		uint32_t m_renderer_id;
		vertex_buffer_layout m_layout;

	};
}
