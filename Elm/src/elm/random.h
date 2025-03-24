#pragma once

#include <glm/glm.hpp>
#include <stdint.h>

namespace elm::random {

	extern void init(void);

	/**
	 * @brief Get a random uint32
	 */
	extern uint32_t uint32(void);

	/**
	 * @brief Get a random uint32 in the range [min - max]
	 */
	extern uint32_t uint32(uint32_t min, uint32_t max);

	/**
	 * @brief Get a random number in the range of [0 - 1]
	 * @note 'seed' will be modified so it can be reused
	 */
	extern float f32(void);

	/**
	 * @brief Get a set of 3 normalized random numbers in the range of [-1 - 1]
	 * @note 'seed' will be modified so it can be reused
	 */
	extern glm::vec3 f32_unit_sphere(void);

	// -- Fast random --

	extern uint32_t wang_hash(uint32_t seed);
	extern uint32_t pcg_hash(uint32_t seed);

	/**
	 * @brief Get a random uint32
	 */
	extern uint32_t uint32_fast(uint32_t &seed);

	/**
	 * @brief Get a random uint32 in the range [min - max]
	 */
	extern uint32_t uint32_fast(uint32_t &seed, uint32_t min, uint32_t max);

	/**
	 * @brief Get a random number in the range of [0 - 1]
	 * @note 'seed' will be modified so it can be reused
	 */
	extern float f32_fast(uint32_t &seed);

	/**
	 * @brief Get a set of 3 normalized random numbers in the range of [-1 - 1]
	 * @note 'seed' will be modified so it can be reused
	 */
	extern glm::vec3 f32_unit_sphere_fast(uint32_t &seed);
}
