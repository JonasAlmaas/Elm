#include "orthographic_camera_controller.h"

#include "elm/core/input/input.h"
#include "elm/core/input/key_code.h"

namespace elm {

	orthographic_camera_controller::orthographic_camera_controller(float aspect_ratio, bool rotation)
		: m_aspect_ratio(aspect_ratio),
		m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level),
		m_rotation(rotation)
	{
	}

	void orthographic_camera_controller::on_update(timestep ts)
	{
		if (input::is_key_pressed(key::A)) {
			glm::vec3 pos = m_camera.get_position();
			pos.x -= glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			pos.y -= glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_camera.set_position(pos);
		} else if (elm::input::is_key_pressed(elm::key::D)) {
			glm::vec3 pos = m_camera.get_position();
			pos.x += glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			pos.y += glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_camera.set_position(pos);
		}

		if (elm::input::is_key_pressed(elm::key::W)) {
			glm::vec3 pos = m_camera.get_position();
			pos.x += -glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			pos.y += glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_camera.set_position(pos);
		} else if (elm::input::is_key_pressed(elm::key::S)) {
			glm::vec3 pos = m_camera.get_position();
			pos.x -= -glm::sin(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			pos.y -= glm::cos(glm::radians(m_camera.get_rotation_deg())) * m_translation_speed * ts.get_seconds();
			m_camera.set_position(pos);
		}

		if (m_rotation) {
			if (elm::input::is_key_pressed(elm::key::Q)) {
				m_camera.set_rotation_deg(m_camera.get_rotation_deg() + m_rotation_speed * ts.get_seconds());
			}
			else if (elm::input::is_key_pressed(elm::key::E)) {
				m_camera.set_rotation_deg(m_camera.get_rotation_deg() - m_rotation_speed * ts.get_seconds());
			}
		}
	}

	void orthographic_camera_controller::on_event(event &e)
	{
		event_dispatcher dispatcher(e);
		dispatcher.dispatch<mouse_scrolled_event>(ELM_BIND_EVENT_FN(orthographic_camera_controller::on_mouse_scrolled));
		dispatcher.dispatch<window_resize_event>(ELM_BIND_EVENT_FN(orthographic_camera_controller::on_window_resize_event));
	}

	bool orthographic_camera_controller::on_mouse_scrolled(mouse_scrolled_event &e)
	{
		m_zoom_level -= e.get_offset_y() * 0.25f;
		m_zoom_level = std::max(m_zoom_level, 0.25f);
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);

		// Maybe use some kinda curve?
		m_translation_speed = m_zoom_level;

		return false;
	}

	bool orthographic_camera_controller::on_window_resize_event(window_resize_event &e)
	{
		m_aspect_ratio = (float)e.get_width() / (float)e.get_height();
		m_camera.set_projection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level);
		return false;
	}
}
