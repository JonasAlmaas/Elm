#pragma once

#include "perspective_camera.h"
#include "elm/core/event/event.h"
#include "elm/core/event/mouse_event.h"
#include "elm/core/timestep.h"
#include <glm/glm.hpp>

namespace elm {

	class perspective_camera_controller
	{
	public:
		perspective_camera_controller(float fov, float aspect_ratio, float near_clip = 0.01f, float far_clip = 10'000.0f);

		void on_update(timestep ts);
		void on_event(event &e);

		void resize_viewport(uint32_t width, uint32_t height);

		inline glm::vec3 get_position(void) const { return m_position; }
		inline float get_pitch_rad(void) const { return m_pitch_rad; }
		inline float get_pitch_deg(void) const { return glm::degrees(m_pitch_rad); }
		inline float get_yaw_rad(void) const { return m_yaw_rad; }
		inline float get_yaw_deg(void) const { return glm::degrees(m_yaw_rad); }
		inline float get_roll_rad(void) const { return m_roll_rad; }
		inline float get_roll_deg(void) const { return glm::degrees(m_roll_rad); }

		void set_position(const glm::vec3 &pos) { m_position = pos; recalculate_view_matrix(); }
		void set_pitch_rad(float pitch) { m_pitch_rad = pitch; recalculate_view_matrix(); }
		void set_pitch_deg(float pitch) { m_pitch_rad = glm::radians(pitch); recalculate_view_matrix(); }
		void set_yaw_rad(float yaw) { m_yaw_rad = yaw; recalculate_view_matrix(); }
		void set_yaw_deg(float yaw) { m_yaw_rad = glm::radians(yaw); recalculate_view_matrix(); }
		void set_roll_rad(float roll) { m_roll_rad = roll; recalculate_view_matrix(); }
		void set_roll_deg(float roll) { m_roll_rad = glm::radians(roll); recalculate_view_matrix(); }

	public:
		inline perspective_camera *get_camera(void) { return &m_camera; }
		inline const perspective_camera *get_camera(void) const { return &m_camera; }

	private:
		bool on_mouse_move_event(mouse_moved_event &e);

		void recalculate_view_matrix(void);

	private:
		perspective_camera m_camera;

		float m_movement_speed = 2.0f;
		float m_sensitivity = 0.003f;

		glm::vec3 m_position = glm::vec3(0.0f);
		float m_pitch_rad = 0.0f;
		float m_yaw_rad = 0.0f;
		float m_roll_rad = 0.0f;

	private:
		float m_last_mouse_x = 0.0f;
		float m_last_mouse_y = 0.0f;

		bool m_is_paning = false;

		bool m_is_moving_left = false;
		bool m_is_moving_right = false;
		bool m_is_moving_forward = false;
		bool m_is_moving_backwards = false;
		bool m_is_moving_up = false;
		bool m_is_moving_down = false;
	};
}
