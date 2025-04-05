#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/camera.hpp"
#include <glm/glm.hpp>

namespace elm {

	struct orthographic_camera : camera
	{
		orthographic_camera(float left, float right, float bottom, float top);

		void set_projection(float left, float right, float bottom, float top);
		void set_view_matrix(const glm::mat4 &view_matrix);

	public:
		inline virtual const glm::mat4 &get_view(void) const override { return this->view; }
		inline virtual const glm::mat4 &get_projection(void) const override { return this->projection; }
		inline virtual const glm::mat4 &get_view_projection(void) const override { return this->view_projection; }

	private:
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 view_projection; /* Cached (projection * view)*/
	};
}
