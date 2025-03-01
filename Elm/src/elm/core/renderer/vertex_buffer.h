#pragma once

#include "elm/core/base.h"
#include <stdint.h>

namespace elm {

	class vertex_buffer
	{
	public:
		virtual ~vertex_buffer(void) {}

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

	public:
		static vertex_buffer *create(const void *vertices, uint32_t size);

	};
}
