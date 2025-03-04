#pragma once

#include "elm/core/base.h"
#include <chrono>

namespace elm {

	class timer
	{
	public:
		timer()
		{
			reset();
		}

		inline void reset(void)
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		inline float elapsed_seconds(void) const
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_start)
				.count() * 0.001f * 0.001f * 0.001f;
		}

		inline float elapsed_milliseconds(void) const
		{
			return elapsed_milliseconds() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
}
