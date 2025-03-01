#pragma once

#include "elm/core/base.h"
#include <memory>

namespace elm {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void init(void) = 0;
		virtual void swap_buffers(void) = 0;

	public:
		static std::unique_ptr<GraphicsContext> create(void *window);

	};
}
