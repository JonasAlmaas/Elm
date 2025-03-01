#pragma once

#include "elm/core/renderer/renderer_api.h"

namespace elm {

	class opengl_renderer_api : public renderer_api
	{
	public:
		virtual void set_clear_color(const glm::vec4 &color) override;
		virtual void clear(void) override;

		virtual void draw_indexed(const std::shared_ptr<vertex_array> &vertex_array) override;

	};
}
