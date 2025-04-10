#include "renderer_info_panel.hpp"
#include <elm.hpp>
#include <imgui.h>
#include <string>

namespace elm {

	void renderer_info_panel::on_imgui_render(void)
	{
		ImGui::Begin("Renderer");
		
		static std::string vendor = render_command::get_vendor();
		static std::string renderer = render_command::get_render();
		static std::string renderer_version = render_command::get_render_version();
		
		ImGui::Text("Vendor: %s", vendor.c_str());
		ImGui::Text("Renderer: %s", renderer.c_str());
		ImGui::Text("Version: %s", renderer_version.c_str());
		ImGui::Text("Frame time: %.3fms", application::get()->get_telemetry()->get_smooth_frame_time_s() * 1000.0f);
		ImGui::Text("FPS: %.2f", application::get()->get_telemetry()->get_fps());
		
		ImGui::End();
	}
}
