#pragma once

#include "elm/core/base.hpp"
#include "elm/core/timestep.hpp"

namespace elm {

	struct application_telemetry
	{
		float get_fps() const {return 1.0f / this->smooth_frame_time_s;};
		float get_smooth_frame_time_s() const {return this->smooth_frame_time_s;};

	public:
		void on_update(timestep ts);

	private:
		float smooth_frame_time_s;
	};
}
