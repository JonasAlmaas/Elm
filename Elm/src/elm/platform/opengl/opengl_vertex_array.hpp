#pragma once

#include "elm/core/renderer/vertex_array.hpp"
#include <stdint.h>
#include <vector>

namespace elm {

	struct opengl_vertex_array final : vertex_array
	{
		opengl_vertex_array();
		virtual ~opengl_vertex_array();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer) override;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer) override;

		virtual const std::vector<std::shared_ptr<vertex_buffer>> *get_vertex_buffers() const override { return &this->vbs; }
		virtual const std::shared_ptr<index_buffer> &get_index_buffer() const override { return this->ib; };

	private:
		uint32_t renderer_id;
		std::vector<std::shared_ptr<vertex_buffer>> vbs;
		std::shared_ptr<index_buffer> ib;
	};
}
