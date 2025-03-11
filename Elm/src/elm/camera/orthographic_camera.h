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

	public:
		inline virtual const glm::mat4 &get_view(void) const override { return m_view_matrix; }
		inline virtual const glm::mat4 &get_projection(void) const override { return m_projection_matrix; }
		inline virtual const glm::mat4 &get_view_projection(void) const override { return m_view_projection_matrix; }

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix; /* Cached (m_projection_matrix * m_view_matrix)*/
	};
}
