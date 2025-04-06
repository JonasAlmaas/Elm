#pragma once

namespace elm {

	struct timestep
	{
		timestep(float time = 0.0f): time_sec(time) {}

		inline float get_seconds(void) const { return this->time_sec; }
		inline float get_milliseconds(void) const { return this->time_sec * 1000.0f; }

	private:
		float time_sec;
	};
}
