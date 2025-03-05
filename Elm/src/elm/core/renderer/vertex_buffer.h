#pragma once

#include "elm/core/base.h"
#include "vertex_buffer_layout.h"
#include <memory>
#include <stdint.h>

namespace elm {

	class vertex_buffer
	{
	public:
		virtual ~vertex_buffer(void) {}

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual void set_data(const void *data, uint32_t size) = 0;

		virtual const vertex_buffer_layout *get_layout(void) const = 0;
		virtual void set_layout(const vertex_buffer_layout *layout) = 0;

	public:
		static std::shared_ptr<vertex_buffer> create(uint32_t size);
		static std::shared_ptr<vertex_buffer> create(const void *vertices, uint32_t size);
	};
}
