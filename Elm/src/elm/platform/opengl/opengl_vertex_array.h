#pragma once

#include "elm/core/renderer/vertex_array.h"
#include <stdint.h>
#include <vector>

namespace elm {

	class opengl_vertex_array : public vertex_array
	{
	public:
		opengl_vertex_array();
		virtual ~opengl_vertex_array() = default;

		virtual void bind(void) const override;
		virtual void unbind(void) const override;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer) override;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer) override;

	private:
		uint32_t m_renderer_id;
		std::vector<std::shared_ptr<vertex_buffer>> m_vertex_buffers;
		std::shared_ptr<index_buffer> m_index_buffer;

	};
}
