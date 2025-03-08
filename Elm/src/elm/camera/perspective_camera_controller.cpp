#include "perspective_camera_controller.h"

#include "elm/core/input/input.h"

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
		if (!is_moving_right && input::is_key_pressed(key::A)) {
			is_moving_left = true;
			/*m_position.x -= glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_position.y -= glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();*/
			m_position.x -= m_translation_speed * ts.get_seconds();
			recalculate_view_matrix();
		} else {
			is_moving_left = false;
		}

		if (!is_moving_left && elm::input::is_key_pressed(elm::key::D)) {
			is_moving_right = true;
			/*pos.x += glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			pos.y += glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();*/
			m_position.x += m_translation_speed * ts.get_seconds();
			recalculate_view_matrix();
		} else {
			is_moving_right = false;
		}

		if (!is_moving_backwards && elm::input::is_key_pressed(elm::key::W)) {
			is_moving_forward = true;
			/*m_position.x += -glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_position.y += glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();*/
			m_position.y += m_translation_speed * ts.get_seconds();
			recalculate_view_matrix();
		} else {
			is_moving_forward = false;
		}

		if (!is_moving_forward && elm::input::is_key_pressed(elm::key::S)) {
			is_moving_backwards = true;
			/*m_position.x -= -glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_position.y -= glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();*/
			m_position.y -= m_translation_speed * ts.get_seconds();
			recalculate_view_matrix();
		} else {
			is_moving_backwards = false;
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
		return false;
	}

	void perspective_camera_controller::recalculate_view_matrix(void)
	{
		m_camera.set_view_matrix(glm::inverse(
			glm::translate(glm::mat4(1.0f), m_position)
			* glm::toMat4(glm::quat(glm::vec3(-m_pitch_rad, -m_roll_rad, -m_yaw_rad)))));
	}
}
