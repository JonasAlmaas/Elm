#pragma once

#include "elm/core/base.hpp"
#include "vertex_buffer_utils.hpp"
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

		vertex_buffer_element() = default;

		vertex_buffer_element(shader_data_type type, const std::string& name, bool normalized=false)
			: name(name), type(type), size(shader_data_type_get_size(type)), offset(0), normalized(normalized)
		{
		}

		uint32_t get_component_count() const
		{
			switch (type) {
			case elm::shader_data_type::BOOL: return 1;
			case elm::shader_data_type::INT: return 1;
			case elm::shader_data_type::INT2: return 2;
			case elm::shader_data_type::INT3: return 3;
			case elm::shader_data_type::INT4: return 4;
			case elm::shader_data_type::FLOAT: return 1;
			case elm::shader_data_type::FLOAT2: return 2;
			case elm::shader_data_type::FLOAT3: return 3;
			case elm::shader_data_type::FLOAT4: return 4;
			case elm::shader_data_type::MAT3: return 3; // 3 * float3
			case elm::shader_data_type::MAT4: return 4; // 4 * float3
			}

			ELM_CORE_ASSERT(false, "Unknown shader data type");
			return 0;
		}
	};
}
