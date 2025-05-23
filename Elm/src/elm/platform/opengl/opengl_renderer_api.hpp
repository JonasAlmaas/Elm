#pragma once

#include "elm/core/renderer/renderer_api.hpp"

namespace elm {

	struct opengl_renderer_api final : renderer_api
	{
		virtual void init() override;

		virtual void get_viewport(uint32_t *x, uint32_t *y, uint32_t *width, uint32_t *height) override;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void set_clear_color(const glm::vec4 &color) override;
		virtual void clear() override;

		virtual void set_depth_test(bool enabled) override;
		virtual void set_depth_fn(depth_function fn) override;

		virtual void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t index_count = 0) override;
		virtual void draw_triangles(uint32_t count, uint32_t offset = 0) override;
		virtual void draw_line(const std::shared_ptr<vertex_array> &vertex_array, uint32_t vertex_count) override;

		virtual void set_line_thickness(float thickness) override;

	public:
		virtual std::string get_vendor() override;
		virtual std::string get_render() override;
		virtual std::string get_render_version() override;
	};
}
