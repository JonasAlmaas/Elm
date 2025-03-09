#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/texture.h"
#include <glad/glad.h>

namespace elm::utils {

	extern GLint texture_2d_filtering_to_gl(texture_2d_filter filter);
	extern GLint texture_2d_wrap_to_gl(texture_2d_wrap wrap);
}
