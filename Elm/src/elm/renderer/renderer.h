#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/vertex_array.h"
#include "elm/core/renderer/camera.h"
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	class renderer
	{
	public:
		static void init(void);

		static void on_window_resize(uint32_t width, uint32_t height);

		static void begin_scene(const camera *camera);
		static void end_scene(void);

		static void submit(
			const std::shared_ptr<shader> &shader,
			const std::shared_ptr<vertex_array> &vertex_array,
			const glm::mat4 &transform = glm::mat4(1.0f));

	private:
		struct scene_data {
			glm::mat4 view_projection_matrix;
		};

		static std::unique_ptr<scene_data> s_scene_data;
	};
}
