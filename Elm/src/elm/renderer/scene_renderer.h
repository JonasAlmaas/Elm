#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/camera.h"
#include "elm/scene/scene.h"
#include <memory>

namespace elm::scene_renderer {

	extern void render(std::shared_ptr<scene> scene, const camera *camera);
}
