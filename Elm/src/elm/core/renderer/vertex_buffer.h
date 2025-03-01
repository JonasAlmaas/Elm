#pragma once

#include "elm/core/base.h"
#include <stdint.h>

namespace elm {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer(void) {}

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

	public:
		static VertexBuffer *create(const void *vertices, uint32_t size);

	};
}
