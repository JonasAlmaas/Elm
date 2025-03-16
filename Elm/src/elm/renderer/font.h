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

		inline const struct msdf_data *get_msdf_data(void) { return m_data; }
		inline std::shared_ptr<texture_2d> &get_atlas_texture(void) { return m_atlas_texture; }

	public:
		static std::shared_ptr<font> create(const std::filesystem::path &fpath);
		static std::shared_ptr<font> get_default(void);

	private:
		struct msdf_data *m_data;
		std::shared_ptr<texture_2d> m_atlas_texture;
	};
}
