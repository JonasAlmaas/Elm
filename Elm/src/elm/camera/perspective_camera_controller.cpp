#include "perspective_camera_controller.h"

#include "elm/core/input/input.h"
#include "elm/core/application.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace elm {

	perspective_camera_controller::perspective_camera_controller(float fov, float aspect_ratio, float near_clip, float far_clip)
		: m_camera(fov, aspect_ratio, near_clip, far_clip)
	{
	}

	void perspective_camera_controller::on_update(timestep ts)
	{
		float dirty_view = false;

		if (!m_is_moving_right && input::is_key_pressed(key::A)) {
			m_is_moving_left = true;
			m_position.x -= glm::cos(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			m_position.y -= glm::sin(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			m_is_moving_left = false;
		}

		if (!m_is_moving_left && input::is_key_pressed(key::D)) {
			m_is_moving_right = true;
			m_position.x += glm::cos(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			m_position.y += glm::sin(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			m_is_moving_right = false;
		}

		if (!m_is_moving_backwards && input::is_key_pressed(key::W)) {
			m_is_moving_forward = true;
			m_position.x += -glm::sin(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			m_position.y += glm::cos(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			m_is_moving_forward = false;
		}

		if (!m_is_moving_forward && input::is_key_pressed(key::S)) {
			m_is_moving_backwards = true;
			m_position.x -= -glm::sin(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			m_position.y -= glm::cos(-m_yaw_rad) * m_movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			m_is_moving_backwards = false;
		}

		if (!m_is_moving_down && input::is_key_pressed(key::E)) {
			m_is_moving_up = true;
			m_position.z += m_movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			m_is_moving_up = false;
		}

		if (!m_is_moving_up && input::is_key_pressed(key::Q)) {
			m_is_moving_down = true;
			m_position.z -= m_movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			m_is_moving_down = false;
		}

		if (input::is_mouse_button_pressed(mouse::Button1)) {
			if (!m_is_paning) {
				application::get()->get_window()->disable_cursor();
				m_last_mouse_x = input::get_mouse_x();
				m_last_mouse_y = input::get_mouse_y();
			}
			m_is_paning = true;
		} else {
			if (m_is_paning) {
				application::get()->get_window()->enable_cursor();
			}
			m_is_paning = false;
		}

		if (dirty_view) {
			recalculate_view_matrix();
		}
	}

	void perspective_camera_controller::on_event(event &e)
	{
		event_dispatcher dispatcher(e);

		dispatcher.dispatch<mouse_moved_event>(ELM_BIND_EVENT_FN(perspective_camera_controller::on_mouse_move_event));
	}

	void perspective_camera_controller::resize_viewport(uint32_t width, uint32_t height)
	{
		m_camera.set_aspect_ratio((float)width / (float)height);
	}

	bool perspective_camera_controller::on_mouse_move_event(mouse_moved_event &e)
	{
		if (!m_is_paning) {
			return false;
		}

		float delta_x = e.get_x() - m_last_mouse_x;
		float delta_y = e.get_y() - m_last_mouse_y;

		m_last_mouse_x = e.get_x();
		m_last_mouse_y = e.get_y();

		m_pitch_rad += delta_y * m_sensitivity;
		m_pitch_rad = glm::clamp(m_pitch_rad, -180.0f, 0.0f);

		m_yaw_rad += delta_x * m_sensitivity;

		recalculate_view_matrix();

		return false;
	}

	void perspective_camera_controller::recalculate_view_matrix(void)
	{
		m_camera.set_view_matrix(glm::inverse(
			glm::translate(glm::mat4(1.0f), m_position)
			* glm::toMat4(glm::quat(glm::vec3(-m_pitch_rad, -m_roll_rad, -m_yaw_rad)))));
	}
}
