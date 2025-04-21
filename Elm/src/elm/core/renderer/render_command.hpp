#pragma once

#include "elm/core/base.hpp"
#include "renderer_api.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace elm {

	struct render_command
	{
		static void init() {s_api->init();}

		static void get_viewport(uint32_t *x, uint32_t *y, uint32_t *width, uint32_t *height) {s_api->get_viewport(x, y, width, height);}
		static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {s_api->set_viewport(x, y, width, height);}

		static void set_clear_color(const glm::vec4 &color) {s_api->set_clear_color(color);}
		static void clear() {s_api->clear();}

		static void set_depth_test(bool enabled) {s_api->set_depth_test(enabled);}
		static void set_depth_fn(depth_function fn) {s_api->set_depth_fn(fn);}

		static void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t index_count = 0) {s_api->draw_indexed(vertex_array, index_count);}
		static void draw_triangles(uint32_t count, uint32_t offset = 0) {s_api->draw_triangles(count, offset);}
		static void draw_line(const std::shared_ptr<vertex_array> &vertex_array, uint32_t vertex_count) {s_api->draw_line(vertex_array, vertex_count);}

		static void set_line_thickness(float thickness) {s_api->set_line_thickness(thickness);}

	public:
		static std::string get_vendor() {return s_api->get_vendor();}
		static std::string get_render() {return s_api->get_render();}
		static std::string get_render_version() {return s_api->get_render_version();}

	private:
		static std::unique_ptr<renderer_api> s_api;
	};
}
