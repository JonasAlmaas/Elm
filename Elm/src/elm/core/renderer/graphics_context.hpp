#pragma once

#include "elm/core/base.hpp"
#include <memory>

namespace elm {

	struct graphics_context
	{
		virtual ~graphics_context() = default;

		virtual void init() = 0;
		virtual void swap_buffers() = 0;

	public:
		static std::unique_ptr<graphics_context> create(void *window);
	};
}
