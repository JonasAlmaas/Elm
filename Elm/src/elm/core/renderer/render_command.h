#pragma once

#include "elm/core/base.h"
#include "renderer_api.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

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

		inline static void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t count = 0)
		{
			s_renderer_api->draw_indexed(vertex_array, count);
		}

		inline static void draw_arrays(uint32_t count, uint32_t offset = 0)
		{
			s_renderer_api->draw_arrays(count, offset);
		}

	public:
		inline static std::string get_vendor(void)
		{
			return s_renderer_api->get_vendor();
		}

		inline static std::string get_render(void)
		{
			return s_renderer_api->get_render();
		}

		inline static std::string get_render_version(void)
		{
			return s_renderer_api->get_render_version();
		}

	private:
		static std::unique_ptr<renderer_api> s_renderer_api;
	};
}
