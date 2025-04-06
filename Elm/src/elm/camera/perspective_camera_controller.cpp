#include "perspective_camera_controller.hpp"

#include "elm/core/input.hpp"
#include "elm/core/application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace elm {

	perspective_camera_controller::perspective_camera_controller(float fov, float aspect_ratio, float near_clip, float far_clip)
		: camera(fov, aspect_ratio, near_clip, far_clip)
	{
	}

	void perspective_camera_controller::on_update(timestep ts)
	{
		float dirty_view = false;

		if (!this->is_moving_right && input::key_pressed(key::A)) {
			this->is_moving_left = true;
			this->position.x -= glm::cos(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			this->position.y -= glm::sin(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			this->is_moving_left = false;
		}

		if (!this->is_moving_left && input::key_pressed(key::D)) {
			this->is_moving_right = true;
			this->position.x += glm::cos(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			this->position.y += glm::sin(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			this->is_moving_right = false;
		}

		if (!this->is_moving_backwards && input::key_pressed(key::W)) {
			this->is_moving_forward = true;
			this->position.x += -glm::sin(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			this->position.y += glm::cos(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			this->is_moving_forward = false;
		}

		if (!this->is_moving_forward && input::key_pressed(key::S)) {
			this->is_moving_backwards = true;
			this->position.x -= -glm::sin(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			this->position.y -= glm::cos(-this->yaw_rad) * this->movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			this->is_moving_backwards = false;
		}

		if (!this->is_moving_down && input::key_pressed(key::E)) {
			this->is_moving_up = true;
			this->position.z += this->movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			this->is_moving_up = false;
		}

		if (!this->is_moving_up && input::key_pressed(key::Q)) {
			this->is_moving_down = true;
			this->position.z -= this->movement_speed * ts.get_seconds();
			dirty_view = true;
		} else {
			this->is_moving_down = false;
		}

		if (input::mouse_button_pressed(mouse::BUTTON1)) {
			if (!this->is_paning) {
				application::get()->get_window()->disable_cursor();
				this->last_mouse_x = input::get_mouse_x();
				this->last_mouse_y = input::get_mouse_y();
			}
			this->is_paning = true;
		} else {
			if (this->is_paning) {
				application::get()->get_window()->enable_cursor();
			}
			this->is_paning = false;
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
		this->camera.set_aspect_ratio((float)width / (float)height);
	}

	bool perspective_camera_controller::on_mouse_move_event(mouse_moved_event &e)
	{
		if (!this->is_paning) {
			return false;
		}

		float delta_x = e.get_x() - this->last_mouse_x;
		float delta_y = e.get_y() - this->last_mouse_y;

		this->last_mouse_x = e.get_x();
		this->last_mouse_y = e.get_y();

		this->pitch_rad += delta_y * this->sensitivity;
		this->pitch_rad = glm::clamp(this->pitch_rad, -180.0f, 0.0f);

		this->yaw_rad += delta_x * this->sensitivity;

		recalculate_view_matrix();

		return false;
	}

	void perspective_camera_controller::recalculate_view_matrix(void)
	{
		this->camera.set_view_matrix(glm::inverse(
			glm::translate(glm::mat4(1.0f), this->position)
			* glm::toMat4(glm::quat(glm::vec3(-this->pitch_rad, -this->roll_rad, -this->yaw_rad)))));
	}
}
