#pragma once

#include "elm/core/base.h"
#include "vertex_array.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace elm {

	enum class depth_function {
		NEVER,
		ALWAYS,
		EQUAL,
		NOTEQUAL,
		LESS, // Default
		GREATER,
		LESSEQUAL,
		GREATEREQUAL
	};

	struct renderer_api
	{
		enum class api {
			none = 0,
			opengl
		};

		virtual ~renderer_api(void) = default;

		virtual void init(void) = 0;

		virtual void get_viewport(uint32_t *x, uint32_t *y, uint32_t *width, uint32_t *height) = 0;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void set_clear_color(const glm::vec4 &color) = 0;
		virtual void clear(void) = 0;

		virtual void set_depth_test(bool enabled) = 0;
		virtual void set_depth_fn(depth_function fn) = 0;

		virtual void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t index_count = 0) = 0;
		virtual void draw_triangles(uint32_t count, uint32_t offset = 0) = 0;
		virtual void draw_line(const std::shared_ptr<vertex_array> &vertex_array, uint32_t vertex_count) = 0;

		virtual void set_line_thickness(float thickness) = 0;

	public:
		virtual std::string get_vendor(void) = 0;
		virtual std::string get_render(void) = 0;
		virtual std::string get_render_version(void) = 0;

	public:
		inline static api get_api(void) { return s_api; }

	private:
		static api s_api;
	};
}
