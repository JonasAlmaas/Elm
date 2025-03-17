#include "opengl_utils.h"

namespace elm::utils {

	extern GLint texture_2d_filtering_to_gl(texture_filter filter)
	{
		switch (filter) {
		case elm::texture_filter::NEAREST: return GL_NEAREST;
		case elm::texture_filter::LINEAR: return GL_LINEAR;
		case elm::texture_filter::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
		default:
			ELM_CORE_ASSERT(false, "Unknown texture 2d filter");
			return GL_LINEAR;
		}
	}

	extern GLint texture_2d_wrap_to_gl(texture_wrap wrap)
	{
		switch (wrap) {
		case elm::texture_wrap::CLAMP: return GL_CLAMP;
		case elm::texture_wrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case elm::texture_wrap::REPEAT: return GL_REPEAT;
		default:
			ELM_CORE_ASSERT(false, "Unknown texture 2d wrap");
			return GL_REPEAT;
		}
	}
}
