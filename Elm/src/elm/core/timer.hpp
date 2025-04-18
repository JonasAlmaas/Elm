#pragma once

#include "elm/core/base.hpp"
#include <chrono>

namespace elm {

	struct timer
	{
		timer()
		{
			reset();
		}

		inline void reset(void)
		{
			this->start = std::chrono::high_resolution_clock::now();
		}

		inline float elapsed_seconds(void) const
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - this->start)
				.count() * 0.001f * 0.001f * 0.001f;
		}

		inline float elapsed_milliseconds(void) const
		{
			return elapsed_seconds() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}
