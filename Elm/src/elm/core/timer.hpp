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

		void reset()
		{
			this->start = std::chrono::high_resolution_clock::now();
		}

		float elapsed_seconds() const
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - this->start)
				.count() * 0.001f * 0.001f * 0.001f;
		}

		float elapsed_milliseconds() const
		{
			return elapsed_seconds() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}
