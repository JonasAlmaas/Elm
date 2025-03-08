#pragma once

#include "orthographic_camera.h"
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

		inline float get_zoom_level(void) const { return m_zoom_level; }
		inline void set_zoom_level(float level) { m_zoom_level = level; }

		inline orthographic_camera *get_camera(void) { return &m_camera; }
		inline const orthographic_camera *get_camera(void) const { return &m_camera; }

		void resize_viewport(uint32_t width, uint32_t height);

	private:
		bool on_mouse_scrolled(mouse_scrolled_event &e);

	private:
		float m_aspect_ratio;
		float m_zoom_level = 1.0f;
		orthographic_camera m_camera;

		bool m_rotation;

		float m_translation_speed = 1.0f;
		float m_rotation_speed = 50.0f;
	};
}
