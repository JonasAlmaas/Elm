#pragma once

#include "perspective_camera.hpp"
#include "elm/core/event/event.hpp"
#include "elm/core/event/mouse_event.hpp"
#include "elm/core/timestep.hpp"
#include <glm/glm.hpp>

namespace elm {

	struct perspective_camera_controller
	{
		perspective_camera_controller(float fov, float aspect_ratio, float near_clip = 0.01f, float far_clip = 10'000.0f);

		void on_update(timestep ts);
		void on_event(event &e);

		void resize_viewport(uint32_t width, uint32_t height);

		inline glm::vec3 get_position(void) const { return this->position; }
		inline float get_pitch_rad(void) const { return this->pitch_rad; }
		inline float get_pitch_deg(void) const { return glm::degrees(this->pitch_rad); }
		inline float get_yaw_rad(void) const { return this->yaw_rad; }
		inline float get_yaw_deg(void) const { return glm::degrees(this->yaw_rad); }
		inline float get_roll_rad(void) const { return this->roll_rad; }
		inline float get_roll_deg(void) const { return glm::degrees(this->roll_rad); }

		void set_position(const glm::vec3 &pos) { this->position = pos; recalculate_view_matrix(); }
		void set_pitch_rad(float pitch) { this->pitch_rad = pitch; recalculate_view_matrix(); }
		void set_pitch_deg(float pitch) { this->pitch_rad = glm::radians(pitch); recalculate_view_matrix(); }
		void set_yaw_rad(float yaw) { this->yaw_rad = yaw; recalculate_view_matrix(); }
		void set_yaw_deg(float yaw) { this->yaw_rad = glm::radians(yaw); recalculate_view_matrix(); }
		void set_roll_rad(float roll) { this->roll_rad = roll; recalculate_view_matrix(); }
		void set_roll_deg(float roll) { this->roll_rad = glm::radians(roll); recalculate_view_matrix(); }

	public:
		inline perspective_camera *get_camera(void) { return &this->camera; }
		inline const perspective_camera *get_camera(void) const { return &this->camera; }

	private:
		bool on_mouse_move_event(mouse_moved_event &e);

		void recalculate_view_matrix(void);

	private:
		perspective_camera camera;

		float movement_speed = 2.0f;
		float sensitivity = 0.003f;

		glm::vec3 position = glm::vec3(0.0f);
		float pitch_rad = 0.0f;
		float yaw_rad = 0.0f;
		float roll_rad = 0.0f;

	private:
		float last_mouse_x = 0.0f;
		float last_mouse_y = 0.0f;

		bool is_paning = false;

		bool is_moving_left = false;
		bool is_moving_right = false;
		bool is_moving_forward = false;
		bool is_moving_backwards = false;
		bool is_moving_up = false;
		bool is_moving_down = false;
	};
}
