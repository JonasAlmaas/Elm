#pragma once

#include "elm/core/base.h"
#include <stdint.h>

namespace elm {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer(void) {}

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

		virtual uint32_t get_count(void) const = 0;

	public:
		static IndexBuffer *create(const uint32_t *indices, uint32_t count);

	};
}
