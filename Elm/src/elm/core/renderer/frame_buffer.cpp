#include "frame_buffer.h"

#include "renderer_api.h"
#include "elm/platform/opengl/opengl_frame_buffer.h"

namespace elm {

	std::shared_ptr<frame_buffer> frame_buffer::create(const frame_buffer_specification &spec)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_frame_buffer>(spec);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
