#pragma once

#include "elm/core/base.h"
#include "renderer_api.h"
#include "vertex_array.h"

namespace elm {

	class renderer
	{
	public:
		static void begin_scene(void);
		static void end_scene(void);

		static void submit(const std::shared_ptr<vertex_array> &vertex_array);

		inline static renderer_api::api get_api(void) { return renderer_api::get_api(); }

	};
}
