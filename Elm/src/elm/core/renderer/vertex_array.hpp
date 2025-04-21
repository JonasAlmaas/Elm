#pragma once

#include "elm/core/base.hpp"
#include "index_buffer.hpp"
#include "vertex_buffer.hpp"
#include <memory>
#include <vector>

namespace elm {

	struct vertex_array
	{
		virtual ~vertex_array() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer) = 0;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer) = 0;

		virtual const std::vector<std::shared_ptr<vertex_buffer>> *get_vertex_buffers() const = 0;
		virtual const std::shared_ptr<index_buffer> &get_index_buffer() const = 0;

	public:
		static std::shared_ptr<vertex_array> create();
	};
}
