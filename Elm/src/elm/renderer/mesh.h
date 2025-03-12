#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/vertex_array.h"
#include <filesystem>
#include <memory>

namespace elm {

	struct mesh
	{
		std::shared_ptr<vertex_array> vertex_array;

	public:
		static std::shared_ptr<mesh> create(const std::filesystem::path &fpath);
	};
}
