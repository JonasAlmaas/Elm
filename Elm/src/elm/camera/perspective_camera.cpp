#include "perspective_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace elm {

	perspective_camera::perspective_camera(float fov, float aspect_ratio, float near_clip, float far_clip)
		: fov(fov),
		aspect_ratio(aspect_ratio),
		near_clip(near_clip),
		far_clip(far_clip),
		view(1.0f),
		projection(glm::perspective(glm::radians(this->fov), this->aspect_ratio, this->near_clip, this->far_clip)),
		view_projection(this->projection * this->view)
	{
	}

	void perspective_camera::set_view_matrix(const glm::mat4 &view_matrix)
	{
		this->view = view_matrix;
		this->view_projection = this->projection * this->view;
	}

	void perspective_camera::recalculate_projection_matrix()
	{
		this->projection = glm::perspective(glm::radians(this->fov), this->aspect_ratio, this->near_clip, this->far_clip);
		this->view_projection = this->projection * this->view;
	}
}
