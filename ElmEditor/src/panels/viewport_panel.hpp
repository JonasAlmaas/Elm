#pragma once

#include <elm.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace elm {

	struct viewport_panel
	{
		viewport_panel();

		void on_attach();
		void on_update(timestep ts, const std::shared_ptr<scene> &scene);
		void on_event(event &e);
		void on_imgui_render();

		perspective_camera_controller camera_controller;
		//orthographic_camera_controller camera_controller;
		std::shared_ptr<frame_buffer> frame_buffer;
		glm::vec2 viewport_size = {0.0f, 0.0f};
		bool viewport_focused = false;
		bool viewport_hovered = false;
	};
}
