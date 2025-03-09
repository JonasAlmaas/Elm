#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>

namespace elm::math {

	extern bool decompose_transform(const glm::mat4 &transform, glm::vec3 *translation, glm::vec3 *rotation, glm::vec3 *scale);
}
