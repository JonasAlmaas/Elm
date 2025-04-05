#pragma once

#include "elm/core/base.hpp"
#include "elm/core/timestep.hpp"

namespace elm {

	struct application_telemetry
	{
		inline float get_fps(void) const { return 1.0f / this->smooth_frame_time_s; };
		inline float get_smooth_frame_time_s(void) const { return this->smooth_frame_time_s; };

	public:
		void on_update(timestep ts);

	private:
		float smooth_frame_time_s;
	};
}
