#pragma once

#include "elm/core/base.h"
#include "renderer_api.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace elm {

	struct render_command
	{
		inline static void init(void) { s_api->init(); }

		inline static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_api->set_viewport(x, y, width, height); }

		inline static void set_clear_color(const glm::vec4 &color) { s_api->set_clear_color(color); }
		inline static void clear(void) { s_api->clear(); }

		inline static void set_depth_test(bool enabled) { s_api->set_depth_test(enabled); }
		inline static void set_depth_fn(depth_function fn) { s_api->set_depth_fn(fn); }

		inline static void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t index_count = 0) { s_api->draw_indexed(vertex_array, index_count); }
		inline static void draw_arrays(uint32_t count, uint32_t offset = 0) { s_api->draw_triangles(count, offset); } 
		inline static void draw_line(const std::shared_ptr<vertex_array> &vertex_array, uint32_t vertex_count) { s_api->draw_line(vertex_array, vertex_count); }

		inline static void set_line_thickness(float thickness) { s_api->set_line_thickness(thickness); }

	public:
		inline static std::string get_vendor(void) { return s_api->get_vendor(); }
		inline static std::string get_render(void) { return s_api->get_render(); }
		inline static std::string get_render_version(void) { return s_api->get_render_version(); }

	private:
		static std::unique_ptr<renderer_api> s_api;
	};
}
