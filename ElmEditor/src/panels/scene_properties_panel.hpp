#pragma once

#include <elm.hpp>
#include <memory>

namespace elm {

	struct scene_properties_panel
	{
		void on_imgui_render(const std::shared_ptr<scene> &scene);
	};
}
