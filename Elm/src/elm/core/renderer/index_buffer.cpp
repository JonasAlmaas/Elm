#include "index_buffer.hpp"

#include "renderer_api.hpp"
#include "elm/platform/opengl/opengl_index_buffer.hpp"

namespace elm {

	std::shared_ptr<index_buffer> index_buffer::create(const uint32_t *indices, uint32_t count)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_index_buffer>(indices, count);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
