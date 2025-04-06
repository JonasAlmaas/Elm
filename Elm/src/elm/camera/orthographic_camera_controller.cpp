#include "orthographic_camera_controller.hpp"

#include "elm/core/input.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace elm {

	orthographic_camera_controller::orthographic_camera_controller(float aspect_ratio, bool enable_rotation)
		: aspect_ratio(aspect_ratio),
		camera(
			-this->aspect_ratio*this->zoom_level,
			this->aspect_ratio*this->zoom_level,
			-this->zoom_level,
			this->zoom_level),
		enable_rotation(enable_rotation)
	{
	}

	void orthographic_camera_controller::on_update(timestep ts)
	{
		ELM_PROFILE_FUNCTION();

		bool dirty_view = false;

		if (input::key_pressed(key::A)) {
			this->position.x -= glm::cos(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			this->position.y -= glm::sin(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			dirty_view = true;
		} else if (elm::input::key_pressed(elm::key::D)) {
			this->position.x += glm::cos(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			this->position.y += glm::sin(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			dirty_view = true;
		}

		if (elm::input::key_pressed(elm::key::W)) {
			this->position.x += -glm::sin(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			this->position.y += glm::cos(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			dirty_view = true;
		} else if (elm::input::key_pressed(elm::key::S)) {
			this->position.x -= -glm::sin(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			this->position.y -= glm::cos(glm::radians(this->rotation_deg)) * this->translation_speed * ts.get_seconds();
			dirty_view = true;
		}

		if (this->enable_rotation) {
			if (elm::input::key_pressed(elm::key::Q)) {
				this->rotation_deg += this->rotation_speed * ts.get_seconds();
				dirty_view = true;
			} else if (elm::input::key_pressed(elm::key::E)) {
				this->rotation_deg -= this->rotation_speed * ts.get_seconds();
				dirty_view = true;
			}
		}

		if (dirty_view) {
			if (this->rotation_deg > 180.0f) {
				this->rotation_deg -= 360.0f;
			} else if (this->rotation_deg <= -180.0f) {
				this->rotation_deg += 360.0f;
			}

			recalculate_view_matrix();
		}
	}

	void orthographic_camera_controller::on_event(event &e)
	{
		ELM_PROFILE_FUNCTION();

		event_dispatcher dispatcher(e);
		dispatcher.dispatch<mouse_scrolled_event>(ELM_BIND_EVENT_FN(orthographic_camera_controller::on_mouse_scrolled));
	}

	void orthographic_camera_controller::resize_viewport(uint32_t width, uint32_t height)
	{
		ELM_PROFILE_FUNCTION();

		this->aspect_ratio = (float)width / (float)height;
		this->camera.set_projection(
			-this->aspect_ratio*this->zoom_level,
			this->aspect_ratio*this->zoom_level,
			-this->zoom_level,
			this->zoom_level);
	}

	bool orthographic_camera_controller::on_mouse_scrolled(mouse_scrolled_event &e)
	{
		ELM_PROFILE_FUNCTION();

		this->zoom_level -= e.get_offset_y() * 0.25f;
		this->zoom_level = std::max(this->zoom_level, 0.25f);
		this->camera.set_projection(
			-this->aspect_ratio*this->zoom_level,
			this->aspect_ratio*this->zoom_level,
			-this->zoom_level,
			this->zoom_level);

		// Maybe use some kinda curve?
		this->translation_speed = this->zoom_level;

		return false;
	}

	void orthographic_camera_controller::recalculate_view_matrix(void)
	{
		ELM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation_deg), { 0.0f, 0.0f, 1.0f });
		this->camera.set_view_matrix(glm::inverse(transform));
	}
}
