#pragma once

#include "elm/core/base.hpp"
#include <memory>
#include <stdint.h>

namespace elm {

	struct index_buffer
	{
		virtual ~index_buffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_count() const = 0;

	public:
		static std::shared_ptr<index_buffer> create(const uint32_t *indices, uint32_t count);
	};
}
