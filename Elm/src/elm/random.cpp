#include "random.h"
#include <random>

namespace elm::random {

	static thread_local std::mt19937 s_random_engine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;

	extern void init(void)
	{
		s_random_engine.seed(std::random_device()());
	}

	extern uint32_t uint32(void)
	{
		return s_distribution(s_random_engine);
	}

	extern uint32_t uint32(uint32_t min, uint32_t max)
	{
		return min + (s_distribution(s_random_engine) % (max - min + 1));
	}

	extern float f32(void)
	{
		return (float)s_distribution(s_random_engine) / (float)UINT32_MAX;
	}

	extern glm::vec3 f32_unit_sphere(void)
	{
		return glm::normalize(glm::vec3(
			f32() * 2.0f - 1.0f,
			f32() * 2.0f - 1.0f,
			f32() * 2.0f - 1.0f));
	}

	// -- Fast random --

	extern uint32_t wang_hash(uint32_t seed)
	{
		seed = (seed ^ 61) ^ (seed >> 16);
		seed *= 9;
		seed = seed ^ (seed >> 4);
		seed *= 0x27D4EB2Du;
		seed = seed ^ (seed >> 15);
		return seed;
	}

	extern uint32_t pcg_hash(uint32_t seed)
	{
		uint32_t state = seed * 747796405u + 2891336453u;
		uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
		return (word >> 22u) ^ word;
	}

	extern uint32_t uint32_fast(uint32_t &seed)
	{
		return pcg_hash(seed);
	}

	extern uint32_t uint32_fast(uint32_t &seed, uint32_t min, uint32_t max)
	{
		seed = pcg_hash(seed);
		return min + (seed % (max - min + 1));
	}

	extern float f32_fast(uint32_t &seed)
	{
		seed = (float)pcg_hash(seed);
		return (float)seed / (float)UINT32_MAX;
	}

	glm::vec3 f32_unit_sphere_fast(uint32_t &seed)
	{
		return glm::normalize(glm::vec3(
			f32_fast(seed) * 2.0f - 1.0f,
			f32_fast(seed) * 2.0f - 1.0f,
			f32_fast(seed) * 2.0f - 1.0f));
	}
}
