#pragma once

#include "elm/core/base.h"

namespace elm {

	enum class renderer_api {
		none = 0,
		opengl
	};

	class renderer
	{
	public:
		inline static renderer_api get_api(void) { return s_renderer_api; }

	public:
		static renderer_api s_renderer_api;

	};
}
