#include "vertex_array.hpp"

#include "renderer_api.hpp"
#include "elm/platform/opengl/opengl_vertex_array.hpp"

namespace elm {

	std::shared_ptr<vertex_array> vertex_array::create()
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
