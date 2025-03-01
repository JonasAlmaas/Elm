#pragma once

#include "elm/core/base.h"
#include <stdint.h>

namespace elm {

	enum class shader_data_type {
		None = 0,
		Bool,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
	};

	static uint32_t shader_data_type_get_size(shader_data_type type)
	{
		switch (type) {
		case shader_data_type::Bool: return 1;
		case shader_data_type::Int: return 4;
		case shader_data_type::Int2: return 4 * 2;
		case shader_data_type::Int3: return 4 * 3;
		case shader_data_type::Int4: return 4 * 4;
		case shader_data_type::Float: return 4;
		case shader_data_type::Float2: return 4 * 2;
		case shader_data_type::Float3: return 4 * 3;
		case shader_data_type::Float4: return 4 * 4;
		case shader_data_type::Mat3: return 4 * 3 * 3;
		case shader_data_type::Mat4: return 4 * 4 * 4;
		}

		ELM_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}
}
