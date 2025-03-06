#pragma once

#include "elm/core/base.h"
#include "vertex_buffer_utils.h"
#include <stddef.h>
#include <stdint.h>
#include <string>

namespace elm {

	struct vertex_buffer_element
	{
		std::string name;
		shader_data_type type;
		uint32_t size;
		size_t offset;
		bool normalized;

		vertex_buffer_element(void) = default;

		vertex_buffer_element(shader_data_type type, const std::string& name, bool normalized=false)
			: name(name), type(type), size(shader_data_type_get_size(type)), offset(0), normalized(normalized)
		{
		}

		uint32_t get_component_count(void) const
		{
			switch (type) {
			case elm::shader_data_type::Bool: return 1;
			case elm::shader_data_type::Int: return 1;
			case elm::shader_data_type::Int2: return 2;
			case elm::shader_data_type::Int3: return 3;
			case elm::shader_data_type::Int4: return 4;
			case elm::shader_data_type::Float: return 1;
			case elm::shader_data_type::Float2: return 2;
			case elm::shader_data_type::Float3: return 3;
			case elm::shader_data_type::Float4: return 4;
			case elm::shader_data_type::Mat3: return 3; // 3 * float3
			case elm::shader_data_type::Mat4: return 4; // 4 * float3
			}

			ELM_CORE_ASSERT(false, "Unknown shader data type");
			return 0;
		}
	};
}
