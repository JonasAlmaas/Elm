#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/shader.h"
#include "elm/core/renderer/vertex_array.h"
#include "elm/core/renderer/camera.h"
#include <glm/glm.hpp>
#include <memory>

namespace elm::renderer {

	extern void init(void);

	extern void on_viewport_resize(uint32_t width, uint32_t height);

	extern void begin_scene(const camera *camera);
	extern void end_scene(void);

	extern void submit(
		const std::shared_ptr<shader> &shader,
		const std::shared_ptr<vertex_array> &vertex_array,
		const glm::mat4 &transform = glm::mat4(1.0f));
}
