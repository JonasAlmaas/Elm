#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/texture.h"
#include <filesystem>
#include <memory>

namespace elm::cubemap {

	extern std::shared_ptr<texture_cube> create(const std::filesystem::path &fpath);
}
