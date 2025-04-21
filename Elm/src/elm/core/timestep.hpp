#pragma once

namespace elm {

	struct timestep
	{
		timestep(float time = 0.0f): time_sec(time) {}

		float get_seconds() const {return this->time_sec;}
		float get_milliseconds() const {return this->time_sec * 1000.0f;}

	private:
		float time_sec;
	};
}
