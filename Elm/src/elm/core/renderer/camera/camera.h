#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>

namespace elm {

	class camera
	{
	public:
		virtual ~camera(void) = default;

		virtual const glm::mat4 &get_view_projection_matrix(void) const = 0;

	};
}
