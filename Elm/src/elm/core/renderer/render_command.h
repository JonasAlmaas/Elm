#pragma once

#include "elm/core/base.h"
#include "renderer_api.h"
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	class render_command
	{
	public:
		inline static void set_clear_color(const glm::vec4 &color) { s_renderer_api->set_clear_color(color); }
		inline static void clear(void) { s_renderer_api->clear(); }

		inline static void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array) { s_renderer_api->draw_indexed(vertex_array); }

	private:
		static std::unique_ptr<renderer_api> s_renderer_api;

	};

}
