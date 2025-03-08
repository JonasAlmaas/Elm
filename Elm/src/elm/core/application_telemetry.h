#pragma once

#include "elm/core/base.h"
#include "elm/core/timestep.h"

namespace elm {

	class application_telemetry
	{
	public:
		inline float get_fps(void) const { return 1.0f / m_smooth_frame_time; };
		inline float get_smooth_frame_time(void) const { return m_smooth_frame_time; };

	public:
		void on_update(timestep ts);

	private:
		float m_smooth_frame_time;
	};
}
