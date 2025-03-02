#pragma once

#include "elm/core/base.h"
#include "renderer_api.h"
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	class render_command
	{
	public:
		inline static void init(void)
		{
			s_renderer_api->init();
		}

		inline static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_renderer_api->set_viewport(x, y, width, height);
		}

		inline static void set_clear_color(const glm::vec4 &color)
		{
			s_renderer_api->set_clear_color(color);
		}

		inline static void clear(void)
		{
			s_renderer_api->clear();
		}

		inline static void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array)
		{
			s_renderer_api->draw_indexed(vertex_array);
		}

	private:
		static std::unique_ptr<renderer_api> s_renderer_api;
	};
}
