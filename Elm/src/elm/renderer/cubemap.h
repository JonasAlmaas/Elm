#pragma once

#include "elm/core/base.h"
#include <filesystem>
#include <memory>

namespace elm {

	struct cubemap
	{

	public:
		static std::shared_ptr<cubemap> create(const std::filesystem::path &fpath);
	};
}
