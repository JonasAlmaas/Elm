#pragma once

#include <stdint.h>

namespace elm {

	/* I am not sure this is implemented correctly, sue me! */
	extern uint32_t crc32(uint8_t *buf, uint32_t size);
}
