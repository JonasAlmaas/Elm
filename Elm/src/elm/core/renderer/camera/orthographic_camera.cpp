#include "orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace elm {

	orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
		: m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_position(0.0f, 0.0f, 0.0f),
		m_view_matrix(1.0f)
	{
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void orthographic_camera::set_position(const glm::vec3 &pos)
	{
		m_position = pos;
		recalculate_view_matrix();
	}

	void orthographic_camera::set_rotation_deg(float rotation_deg)
	{
		m_rotation_deg = rotation_deg;
		recalculate_view_matrix();
	}

	void orthographic_camera::recalculate_view_matrix(void)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation_deg), { 0.0f, 0.0f, 1.0f });

		m_view_matrix = glm::inverse(transform);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}
}
