#include "uniform_buffer.hpp"

#include "renderer_api.hpp"
#include "elm/platform/opengl/opengl_uniform_buffer.hpp"

namespace elm {

	std::shared_ptr<uniform_buffer> uniform_buffer::create(uint32_t size, uint32_t binding)
	{
		switch (renderer_api::get_api()) {
		case renderer_api::api::none: ELM_CORE_ASSERT(false, "renderer_api::api::none is not currently supported"); break;
		case renderer_api::api::opengl:
			return std::make_shared<opengl_uniform_buffer>(size, binding);
		}

		ELM_CORE_ASSERT(false, "Unknown renderer API");
		return nullptr;
	}
}
