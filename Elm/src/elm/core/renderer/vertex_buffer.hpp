#pragma once

#include "elm/core/base.hpp"
#include "vertex_buffer_layout.hpp"
#include <memory>
#include <stdint.h>

namespace elm {

	struct vertex_buffer
	{
		virtual ~vertex_buffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void set_data(const void *data, uint32_t size) = 0;

		virtual const vertex_buffer_layout *get_layout() const = 0;
		virtual void set_layout(const vertex_buffer_layout *layout) = 0;

	public:
		static std::shared_ptr<vertex_buffer> create(uint32_t size);
		static std::shared_ptr<vertex_buffer> create(const void *vertices, uint32_t size);
	};
}
