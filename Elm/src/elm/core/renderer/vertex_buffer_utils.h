#pragma once

#include "elm/core/base.h"
#include <stdint.h>

namespace elm {

	enum class shader_data_type {
		NONE = 0,
		BOOL,
		INT, INT2, INT3, INT4,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MAT3, MAT4,
	};

	static uint32_t shader_data_type_get_size(shader_data_type type)
	{
		switch (type) {
		case shader_data_type::BOOL: return 1;
		case shader_data_type::INT: return 4;
		case shader_data_type::INT2: return 4 * 2;
		case shader_data_type::INT3: return 4 * 3;
		case shader_data_type::INT4: return 4 * 4;
		case shader_data_type::FLOAT: return 4;
		case shader_data_type::FLOAT2: return 4 * 2;
		case shader_data_type::FLOAT3: return 4 * 3;
		case shader_data_type::FLOAT4: return 4 * 4;
		case shader_data_type::MAT3: return 4 * 3 * 3;
		case shader_data_type::MAT4: return 4 * 4 * 4;
		}

		ELM_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}
}
