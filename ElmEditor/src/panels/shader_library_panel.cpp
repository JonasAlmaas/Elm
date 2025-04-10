#include "shader_library_panel.hpp"
#include <imgui.h>

namespace elm {

	void shader_library_panel::on_imgui_render(const shader_library &shader_lib)
	{
		ImGui::Begin("Shaeder Library");

		for (auto &[name, shader] : shader_lib.get_shaders()) {
			ImGui::Text(name.c_str());
			if (ImGui::Button("Refresh")) {
				shader->reload();
			}
		}

		ImGui::End();
	}
}
