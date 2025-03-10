#include "application_telemetry.h"
#include <stdint.h>

namespace elm {

	void application_telemetry::on_update(timestep ts)
	{
		static float frame_time_acc = 0.0f;
		static uint32_t frame_time_acc_ix = 0u;

		frame_time_acc += ts.get_seconds();
		++frame_time_acc_ix;

		if (frame_time_acc_ix >= 30) {
			m_smooth_frame_time_s = frame_time_acc / (float)frame_time_acc_ix;
			frame_time_acc = 0.0f;
			frame_time_acc_ix = 0;
		}
	}
}
