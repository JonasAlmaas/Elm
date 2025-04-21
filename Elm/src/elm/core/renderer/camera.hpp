#pragma once

#include "elm/core/base.hpp"
#include <glm/glm.hpp>

namespace elm {

	struct camera
	{
		virtual ~camera() = default;

		virtual const glm::mat4 &get_view() const = 0;
		virtual const glm::mat4 &get_projection() const = 0;
		virtual const glm::mat4 &get_view_projection() const = 0;
	};
}
