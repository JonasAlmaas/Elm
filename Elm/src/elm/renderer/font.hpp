#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/texture.hpp"
#include <filesystem>
#include <memory>

namespace elm {

	struct font
	{
		font(const std::filesystem::path &fpath);
		~font();

		const struct msdf_data *get_msdf_data() {return this->data;}
		std::shared_ptr<texture_2d> &get_atlas_texture() {return this->atlas_texture;}

	public:
		static std::shared_ptr<font> create(const std::filesystem::path &fpath);
		static std::shared_ptr<font> get_default();

	private:
		struct msdf_data *data;
		std::shared_ptr<texture_2d> atlas_texture;
	};
}
