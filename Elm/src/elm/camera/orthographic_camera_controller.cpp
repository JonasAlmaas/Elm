#include "orthographic_camera_controller.h"

#include "elm/core/input/input.h"
#include "elm/core/input/key_code.h"
#include <glm/gtc/matrix_transform.hpp>

namespace elm {

	orthographic_camera_controller::orthographic_camera_controller(float aspect_ratio, bool enable_rotation)
		: m_aspect_ratio(aspect_ratio),
		m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level),
		m_enable_rotation(enable_rotation)
	{
	}

	void orthographic_camera_controller::on_update(timestep ts)
	{
		ELM_PROFILE_FUNCTION();

		bool dirty_view = false;

		if (input::is_key_pressed(key::A)) {
			m_position.x -= glm::cos(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			m_position.y -= glm::sin(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			dirty_view = true;
		} else if (elm::input::is_key_pressed(elm::key::D)) {
			m_position.x += glm::cos(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			m_position.y += glm::sin(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			dirty_view = true;
		}

		if (elm::input::is_key_pressed(elm::key::W)) {
			m_position.x += -glm::sin(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			m_position.y += glm::cos(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			dirty_view = true;
		} else if (elm::input::is_key_pressed(elm::key::S)) {
			m_position.x -= -glm::sin(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			m_position.y -= glm::cos(glm::radians(m_rotation_deg)) * m_translation_speed * ts.get_seconds();
			dirty_view = true;
		}

		if (m_enable_rotation) {
			if (elm::input::is_key_pressed(elm::key::Q)) {
				m_rotation_deg += m_rotation_speed * ts.get_seconds();
				dirty_view = true;
			} else if (elm::input::is_key_pressed(elm::key::E)) {
				m_rotation_deg -= m_rotation_speed * ts.get_seconds();
				dirty_view = true;
			}
		}

		if (dirty_view) {
			if (m_rotation_deg > 180.0f) {
				m_rotation_deg -= 360.0f;
			} else if (m_rotation_deg <= -180.0f) {
				m_rotation_deg += 360.0f;
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

		m_aspect_ratio = (float)width / (float)height;
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
	}

	bool orthographic_camera_controller::on_mouse_scrolled(mouse_scrolled_event &e)
	{
		ELM_PROFILE_FUNCTION();

		m_zoom_level -= e.get_offset_y() * 0.25f;
		m_zoom_level = std::max(m_zoom_level, 0.25f);
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);

		// Maybe use some kinda curve?
		m_translation_speed = m_zoom_level;

		return false;
	}

	void orthographic_camera_controller::recalculate_view_matrix(void)
	{
		ELM_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_deg), { 0.0f, 0.0f, 1.0f });
		m_camera.set_view_matrix(glm::inverse(transform));
	}
}
