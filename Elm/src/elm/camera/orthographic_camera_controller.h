#pragma once

#include "elm/core/renderer/camera/orthographic_camera.h"
#include "elm/core/event/mouse_event.h"
#include "elm/core/event/window_event.h"
#include "elm/core/timestep.h"

namespace elm {

	class orthographic_camera_controller
	{
	public:
		orthographic_camera_controller(float aspect_ratio, bool rotation = false);

		void on_update(timestep ts);
		void on_event(event &e);

		inline orthographic_camera *get_camera(void) { return &m_camera; }
		inline const orthographic_camera *get_camera(void) const { return &m_camera; }

	private:
		bool on_mouse_scrolled(mouse_scrolled_event &e);
		bool on_window_resize_event(window_resize_event &e);

	private:
		float m_aspect_ratio;
		float m_zoom_level = 1.0f;
		orthographic_camera m_camera;

		bool m_rotation;

		float m_translation_speed = 1.0f;
		float m_rotation_speed = 50.0f;
	};
}
