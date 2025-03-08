#include "orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace elm {

	orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
		: m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		m_view_matrix(1.0f)
	{
		ELM_PROFILE_FUNCTION();

		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void orthographic_camera::set_projection(float left, float right, float bottom, float top)
	{
		ELM_PROFILE_FUNCTION();

		m_projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void orthographic_camera::set_view_matrix(const glm::mat4 &view_matrix)
	{
		ELM_PROFILE_FUNCTION();

		m_view_matrix = view_matrix;
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}
}
