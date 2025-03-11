#pragma once

#include "elm/core/base.h"
#include "elm/core/renderer/camera.h"
#include <glm/glm.hpp>

namespace elm {

	class perspective_camera : public camera
	{
	public:
		perspective_camera(void) = delete;
		perspective_camera(float fov, float aspect_ratio, float near_clip = 0.01f, float far_clip = 10'000.0f);

		inline float get_fov(void) const { return m_fov; }
		inline float get_aspect_ratio(void) const { return m_aspect_ratio; }
		inline float get_near_clip(void) const { return m_near_clip; }
		inline float get_far_clip(void) const { return m_far_clip; }

		void set_fov(float fov) { m_fov = fov; recalculate_projection_matrix(); }
		inline void set_aspect_ratio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; recalculate_projection_matrix(); }
		inline void set_near_clip(float near_clip) { m_near_clip = near_clip; recalculate_projection_matrix(); }
		inline void set_far_clip(float far_clip) { m_far_clip = far_clip; recalculate_projection_matrix(); }

		void set_view_matrix(const glm::mat4 &view_matrix);

	public:
		inline virtual const glm::mat4 &get_view(void) const override { return m_view_matrix; }
		inline virtual const glm::mat4 &get_projection(void) const override { return m_projection_matrix; }
		inline virtual const glm::mat4 &get_view_projection(void) const override { return m_view_projection_matrix; }

	private:
		void recalculate_projection_matrix(void);

	private:
		float m_fov = 45.0f;
		float m_aspect_ratio = 16.0f / 9.0f;
		float m_near_clip = 0.1f;
		float m_far_clip = 10000.0f;
		
	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix; /* Cached (m_projection_matrix * m_view_matrix)*/
	};
}
