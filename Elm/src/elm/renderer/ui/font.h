#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/texture.h"
#include <filesystem>
#include <memory>

namespace elm {

	struct font
	{
		font(const std::filesystem::path &fpath);
		~font(void);

	private:
		struct msdf_data *m_data;
		std::shared_ptr<texture_2d> m_atlas_texture;
	};
}
