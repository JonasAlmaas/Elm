#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/camera.hpp"
#include <glm/glm.hpp>

namespace elm {

	struct orthographic_camera final : camera
	{
		orthographic_camera(float left, float right, float bottom, float top);

		void set_projection(float left, float right, float bottom, float top);
		void set_view_matrix(const glm::mat4 &view_matrix);

	public:
		virtual const glm::mat4 &get_view() const override {return this->view;}
		virtual const glm::mat4 &get_projection() const override {return this->projection;}
		virtual const glm::mat4 &get_view_projection() const override {return this->view_projection;}

	private:
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 view_projection; /* Cached (projection * view)*/
	};
}
