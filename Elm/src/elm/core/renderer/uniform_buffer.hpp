#pragma once

#include "elm/core/base.hpp"
#include <memory>
#include <stdint.h>

namespace elm {

	struct uniform_buffer
	{
		virtual ~uniform_buffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void set_data(const void *data, uint32_t size, uint32_t offset=0) = 0;

	public:
		static std::shared_ptr<uniform_buffer> create(uint32_t size, uint32_t binding);
	};
}
