#pragma once

#include "elm/core/base.h"

namespace elm {

	enum class RendererAPI {
		None = 0,
		OpenGL
	};

	class Renderer
	{
	public:
		inline static RendererAPI get_api(void) { return s_renderer_api; }

	public:
		static RendererAPI s_renderer_api;

	};
}
