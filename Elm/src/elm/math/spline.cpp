#include "spline.h"

namespace elm::math {

	static float calculate_segment_length(const spline *s, uint32_t point_ix)
	{
		const float step_size = 0.005f;
		float seg_length = 0.0f;

		glm::vec3 prev_p, p;
		prev_p = s->get_point((float)point_ix);

		for (float t = step_size; t <= 1.0f; t += step_size) {
			p = s->get_point((float)point_ix + t);
			seg_length += glm::distance(p, prev_p);
			prev_p = p;
		}

		return seg_length;
	}

	glm::vec3 spline::get_point(float t) const
	{
		uint32_t p0, p1, p2, p3;
		if (this->looped) {
			p1 = glm::min((uint32_t)t, (uint32_t)(this->points.size() - 1u));
			p2 = (p1 + 1u) % this->points.size();
			p3 = (p2 + 1u) % this->points.size();
			p0 = p1 >= 1u ? p1 - 1u : (uint32_t)this->points.size() - 1u;
			t = t - (float)glm::min((uint32_t)t, (uint32_t)(this->points.size() - 1u));
		} else {
			p1 = glm::min((uint32_t)t + 1u, (uint32_t)(this->points.size() - 3u));
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
			t = t - (float)glm::min((uint32_t)t, (uint32_t)(this->points.size() - 4u));
		}
		
		float tt = t * t;
		float ttt = tt * t;

		float q0 = -ttt + 2.0f * tt - t;
		float q1 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q2 = -3.0f * ttt + 4.0f * tt + t;
		float q3 = ttt - tt;

		return ((glm::vec3)this->points[p0] * q0
			+ (glm::vec3)this->points[p1] * q1
			+ (glm::vec3)this->points[p2] * q2
			+ (glm::vec3)this->points[p3] * q3) * 0.5f;
	}

	glm::vec3 spline::get_forward(float t) const
	{
		uint32_t p0, p1, p2, p3;
		if (this->looped) {
			p1 = glm::min((uint32_t)t, (uint32_t)(this->points.size() - 1u));
			p2 = (p1 + 1u) % this->points.size();
			p3 = (p2 + 1u) % this->points.size();
			p0 = p1 >= 1u ? p1 - 1u : (uint32_t)this->points.size() - 1u;
			t = t - (float)glm::min((uint32_t)t, (uint32_t)(this->points.size() - 1u));
		} else {
			p1 = glm::min((uint32_t)t + 1u, (uint32_t)(this->points.size() - 3u));
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
			t = t - (float)glm::min((uint32_t)t, (uint32_t)(this->points.size() - 4u));
		}

		float tt = t * t;
		float ttt = tt * t;

		float q0 = -3.0f * tt + 4.0f * t - 1;
		float q1 = 9.0f * tt - 10.0f * t;
		float q2 = -9.0f * tt + 8.0f * t + 1.0f;
		float q3 = 3.0f * tt - 2.0f * t;

		glm::vec3 forward = ((glm::vec3)this->points[p0] * q0
			+ (glm::vec3)this->points[p1] * q1
			+ (glm::vec3)this->points[p2] * q2
			+ (glm::vec3)this->points[p3] * q3) * 0.5f;

		return glm::normalize(forward);
	}

	float spline::get_normalised_offset(float p) const
	{
		while (p > this->length) {
			p = this->length;
		}

		int i = 0;
		while (p > this->segment_lengths[i % this->segment_lengths.size()]) {
			p -= this->segment_lengths[i % this->segment_lengths.size()];
			++i;
		}

		return (float)i + (p / this->segment_lengths[i % this->segment_lengths.size()]);
	}

	void spline::invalidate(void)
	{
		ELM_ASSERT(this->points.size() >= 4, "A spline must contain at least 4 points");

		this->length = 0.0f;

		for (uint32_t i = 0; i < this->points.size(); ++i) {
			if (this->looped || i < this->points.size() - 3) {
				float len = calculate_segment_length(this, i);
				this->segment_lengths.push_back(len);
				this->length += len;
			}
		}
	}
}
