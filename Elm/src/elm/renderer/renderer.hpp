#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/shader.hpp"
#include "elm/core/renderer/vertex_array.hpp"
#include "elm/core/renderer/camera.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace elm::renderer {

	extern void init();
	extern void shutdown();

	extern void on_viewport_resize(uint32_t width, uint32_t height);

	extern void begin_scene(const camera *camera);
	extern void end_scene();

	extern void submit(
		const std::shared_ptr<shader> &shader,
		const std::shared_ptr<vertex_array> &vertex_array,
		const glm::mat4 &transform = glm::mat4(1.0f));
}
