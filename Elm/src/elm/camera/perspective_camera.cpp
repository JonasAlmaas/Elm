#include "perspective_camera.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace elm {

	perspective_camera::perspective_camera(float fov, float aspect_ratio, float near_clip, float far_clip)
		: m_fov(fov),
		m_aspect_ratio(aspect_ratio),
		m_near_clip(near_clip),
		m_far_clip(far_clip),
		m_view_matrix(1.0f),
		m_projection_matrix(glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_clip, m_far_clip)),
		m_view_projection_matrix(m_projection_matrix * m_view_matrix)
	{
	}

	void perspective_camera::set_view_matrix(const glm::mat4 &view_matrix)
	{
		m_view_matrix = view_matrix;
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void perspective_camera::recalculate_projection_matrix(void)
	{
		m_projection_matrix = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_clip, m_far_clip);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}
}
