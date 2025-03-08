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
		void set_view_matrix(const glm::mat4 &view_matrix);

		inline const glm::mat4 &get_projection_matrix(void) const { return m_projection_matrix; }
		inline const glm::mat4 &get_view_matrix(void) const { return m_view_matrix; }
		inline virtual const glm::mat4 &get_view_projection_matrix(void) const override { return m_view_projection_matrix; }

	private:
		void recalculate_view_matrix(void);

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix; /* Cached (m_projection_matrix * m_view_matrix)*/
	};
}
