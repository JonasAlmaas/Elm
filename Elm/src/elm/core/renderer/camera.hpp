#pragma once

#include "elm/core/base.hpp"
#include <glm/glm.hpp>

namespace elm {

	struct camera
	{
		virtual ~camera(void) = default;

		virtual const glm::mat4 &get_view(void) const = 0;
		virtual const glm::mat4 &get_projection(void) const = 0;
		virtual const glm::mat4 &get_view_projection(void) const = 0;
	};
}
