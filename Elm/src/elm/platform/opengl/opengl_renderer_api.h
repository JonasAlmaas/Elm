#pragma once

#include "elm/core/renderer/renderer_api.h"

namespace elm {

	class opengl_renderer_api : public renderer_api
	{
	public:
		virtual void init(void) override;

		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void set_clear_color(const glm::vec4 &color) override;
		virtual void clear(void) override;

		virtual void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array, uint32_t count = 0) override;
	};
}
