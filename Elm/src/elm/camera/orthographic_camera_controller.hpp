#pragma once

#include "orthographic_camera.hpp"
#include "elm/core/event/mouse_event.hpp"
#include "elm/core/event/window_event.hpp"
#include "elm/core/timestep.hpp"

namespace elm {

	struct orthographic_camera_controller
	{
		orthographic_camera_controller(float aspect_ratio, bool enable_rotation = false);

		void on_update(timestep ts);
		void on_event(event &e);

		float get_zoom_level() const {return this->zoom_level;}
		void set_zoom_level(float level) {this->zoom_level = level;}

		void resize_viewport(uint32_t width, uint32_t height);

		orthographic_camera *get_camera() {return &this->camera;}
		const orthographic_camera *get_camera() const {return &this->camera;}

	private:
		bool on_mouse_scrolled(mouse_scrolled_event &e);

		void recalculate_view_matrix();

	private:
		float aspect_ratio;
		float zoom_level = 1.0f;
		orthographic_camera camera;
		bool enable_rotation;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		float rotation_deg = 0.0f; // Anti-clockwise

		float translation_speed = 1.0f;
		float rotation_speed = 50.0f;
	};
}
