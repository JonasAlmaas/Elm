#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/texture.hpp"
#include <filesystem>
#include <memory>
#include <stdint.h>

namespace elm::cubemap {

	extern std::shared_ptr<texture_cube> create(const std::filesystem::path &fpath, uint32_t size);
	extern std::shared_ptr<texture_cube> create_irradiance(const std::shared_ptr<texture_cube> cubemap, uint32_t size);
	extern std::shared_ptr<texture_cube> create_prefilter(const std::shared_ptr<texture_cube> cubemap, uint32_t size);
	extern std::shared_ptr<texture_2d> create_brdf_lut_map(uint32_t size);
}
