#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/camera.h"
#include <glm/glm.hpp>

namespace elm {

	class orthographic_camera : public camera
	{
	public:
		orthographic_camera(float left, float right, float bottom, float top);

		void set_projection(float left, float right, float bottom, float top);

		inline const glm::vec3 &get_position(void) const { return m_position; }
		void set_position(const glm::vec3 &pos);

		inline float get_rotation_deg(void) const { return m_rotation_deg; }
		void set_rotation_deg(float rotation_deg);

		inline const glm::mat4 &get_projection_matrix(void) const { return m_projection_matrix; }
		inline const glm::mat4 &get_view_matrix(void) const { return m_view_matrix; }
		inline virtual const glm::mat4 &get_view_projection_matrix(void) const override { return m_view_projection_matrix; }

	private:
		void recalculate_view_matrix(void);

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix; /* Cached (m_projection_matrix * m_view_matrix)*/

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation_deg = 0.0f; // Anti-clockwise
	};
}
