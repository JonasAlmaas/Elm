#pragma once

#include "elm/core/base.hpp"
#include "elm/core/renderer/camera.hpp"
#include <glm/glm.hpp>

namespace elm {

	struct perspective_camera : camera
	{
		perspective_camera() = delete;
		perspective_camera(float fov, float aspect_ratio, float near_clip = 0.01f, float far_clip = 10'000.0f);

		float get_fov() const {return this->fov;}
		float get_aspect_ratio() const {return this->aspect_ratio;}
		float get_near_clip() const {return this->near_clip;}
		float get_far_clip() const {return this->far_clip;}

		void set_fov(float fov) {this->fov = fov; recalculate_projection_matrix();}
		void set_aspect_ratio(float aspect_ratio) {this->aspect_ratio = aspect_ratio; recalculate_projection_matrix();}
		void set_near_clip(float near_clip) {this->near_clip = near_clip; recalculate_projection_matrix();}
		void set_far_clip(float far_clip) {this->far_clip = far_clip; recalculate_projection_matrix();}

		void set_view_matrix(const glm::mat4 &view_matrix);

	public:
		virtual const glm::mat4 &get_view() const override {return this->view;}
		virtual const glm::mat4 &get_projection() const override {return this->projection;}
		virtual const glm::mat4 &get_view_projection() const override {return this->view_projection;}

	private:
		void recalculate_projection_matrix();

	private:
		float fov = 45.0f;
		float aspect_ratio = 16.0f / 9.0f;
		float near_clip = 0.1f;
		float far_clip = 10000.0f;
		
	private:
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 view_projection; /* Cached (projection * view)*/
	};
}
