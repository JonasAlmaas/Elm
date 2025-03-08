#pragma once

#include "elm/core/base.h"
#include "vertex_array.h"
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	class renderer_api
	{
	public:
		enum class api {
			none = 0,
			opengl
		};

	public:
		virtual ~renderer_api(void) = default;

		virtual void init(void) = 0;

		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void set_clear_color(const glm::vec4 &color) = 0;
		virtual void clear(void) = 0;

		virtual void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t count = 0) = 0;

	public:
		inline static api get_api(void) { return s_api; }

	private:
		static api s_api;
	};
}
