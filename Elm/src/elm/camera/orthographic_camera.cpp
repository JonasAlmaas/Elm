#include "orthographic_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace elm {

	orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
		: projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), view(1.0f)
	{
		ELM_PROFILE_FUNCTION();

		this->view_projection = this->projection * this->view;
	}

	void orthographic_camera::set_projection(float left, float right, float bottom, float top)
	{
		ELM_PROFILE_FUNCTION();

		this->projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		this->view_projection = this->projection * this->view;
	}

	void orthographic_camera::set_view_matrix(const glm::mat4 &view_matrix)
	{
		ELM_PROFILE_FUNCTION();

		this->view = view_matrix;
		this->view_projection = this->projection * this->view;
	}
}
