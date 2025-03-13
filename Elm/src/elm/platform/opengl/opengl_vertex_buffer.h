#pragma once

#include "elm/core/renderer/vertex_buffer.h"
#include <stdint.h>
#include <memory>

namespace elm {

	struct opengl_vertex_buffer : vertex_buffer
	{
		opengl_vertex_buffer(uint32_t size);
		opengl_vertex_buffer(const void *vertices, uint32_t size);
		virtual ~opengl_vertex_buffer();

		virtual void bind(void) const override;
		virtual void unbind(void) const  override;

		virtual void set_data(const void *data, uint32_t size) override;

		inline virtual const vertex_buffer_layout *get_layout(void) const override { return &m_layout; }
		virtual void set_layout(const vertex_buffer_layout *layout) override;

	private:
		uint32_t m_renderer_id;
		vertex_buffer_layout m_layout;
	};
}
