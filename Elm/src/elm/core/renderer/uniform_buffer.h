#pragma once

#include "elm/core/base.h"
#include <memory>
#include <stdint.h>

namespace elm {

	class uniform_buffer
	{
	public:
		~uniform_buffer(void) = default;

		virtual void set_data(const void *data, uint32_t size, uint32_t offset = 0) = 0;

	public:
		static std::shared_ptr<uniform_buffer> create(uint32_t size, uint32_t binding);
	};
}
