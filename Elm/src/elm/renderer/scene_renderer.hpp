#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/camera.hpp"
#include "elm/scene/scene.hpp"
#include <memory>

namespace elm::scene_renderer {

	extern void init(void);
	extern void shutdown(void);

	extern void render(std::shared_ptr<scene> scene, const camera *camera);
}
