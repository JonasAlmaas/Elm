#pragma once

#include "elm/core/base.h"
#include <glm/glm.hpp>

namespace elm::math {

	/**
	 * Catmull-Rom Spline
	 * https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline
	 */
	struct spline
	{
		std::vector<glm::vec3> points;
		bool looped = false;

		// Set by invalidate()
		float length = 0.0f;
		std::vector<float> segment_lengths;

		void invalidate(void);

		/**
		 * @param t fractional value
		 * t = 1.5f would be between the second and third spline point.
		 * 
		 * @return vec3 based on t and whether it is looped or not.
		 */
		glm::vec3 get_point(float t) const;

		glm::vec3 get_forward(float t) const;

		/**
		 * @param World distance along the spline.
		 * @return Fractional value to be used as input for get_point(...) and/or get_forward_vector(...)
		 */
		float get_normalised_offset(float p) const;
	};
}
