#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/texture.hpp"
#include <glad/glad.h>

namespace elm::utils {

	extern GLint texture_2d_filtering_to_gl(texture_filter filter);
	extern GLint texture_2d_wrap_to_gl(texture_wrap wrap);
}
