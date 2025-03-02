#pragma once

#include "elm/core/base.h"
#include <memory>

namespace elm {

	class graphics_context
	{
	public:
		virtual ~graphics_context(void) = default;

		virtual void init(void) = 0;
		virtual void swap_buffers(void) = 0;

	public:
		static std::unique_ptr<graphics_context> create(void *window);
	};
}
