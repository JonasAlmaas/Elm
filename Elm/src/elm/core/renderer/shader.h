#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>
#include <string>

namespace elm {

	class shader
	{
	public:
		virtual ~shader(void) = default;

		virtual void bind(void) const = 0;
		virtual void unbind(void) const = 0;

	public:
		static shader *create(const std::string &vertex_src, const std::string &fragment_src);

	};
}
