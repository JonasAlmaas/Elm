#pragma once

#include "elm/core/base.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include <memory>

namespace elm {

	class vertex_array
	{
	public:
		virtual ~vertex_array(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer> &vertex_buffer) = 0;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer> &index_buffer) = 0;

	public:
		static vertex_array *create(void);

	};
}
