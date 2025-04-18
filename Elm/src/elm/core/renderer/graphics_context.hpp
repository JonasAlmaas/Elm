#pragma once

#include "elm/core/base.hpp"
#include <memory>

namespace elm {

	struct graphics_context
	{
		virtual ~graphics_context(void) = default;

		virtual void init(void) = 0;
		virtual void swap_buffers(void) = 0;

	public:
		static std::unique_ptr<graphics_context> create(void *window);
	};
}
