#pragma once

namespace elm {

	class timestep
	{
	public:
		timestep(float time = 0.0f)
			: m_time_sec(time)
		{
		}

		inline float get_seconds(void) const { return m_time_sec; }
		inline float get_milliseconds(void) const { return m_time_sec * 1000.0f; }

	private:
		float m_time_sec;

	};
}
