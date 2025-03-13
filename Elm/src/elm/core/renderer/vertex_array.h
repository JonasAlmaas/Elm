#pragma once

#include "elm/core/base.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include <memory>
#include <vector>

namespace elm {

	struct vertex_array
	{
		virtual ~vertex_array(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer) = 0;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer) = 0;

		virtual const std::vector<std::shared_ptr<vertex_buffer>> *get_vertex_buffers(void) const = 0;
		virtual const std::shared_ptr<index_buffer> &get_index_buffer(void) const = 0;

	public:
		static std::shared_ptr<vertex_array> create(void);
	};
}
