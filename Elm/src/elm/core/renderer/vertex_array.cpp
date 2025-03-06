#include "vertex_array.h"

#include "renderer_api.h"
#include "elm/platform/opengl/opengl_vertex_array.h"

namespace elm {

	std::shared_ptr<vertex_array> vertex_array::create(void)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_vertex_array>();
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
