#include "crc32.h"

namespace elm {

	static uint32_t reverse(uint32_t x) {
		x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
		x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
		x = ((x & 0x0F0F0F0F) << 4) | ((x >> 4) & 0x0F0F0F0F);
		x = (x << 24) | ((x & 0xFF00) << 8) |
			((x >> 8) & 0xFF00) | (x >> 24);
		return x;
	}

	extern uint32_t crc32(uint8_t *buf, uint32_t size) {
		uint32_t i, j;
		uint32_t byte, crc;

		crc = 0xFFFFFFFFu;
		for (i = 0u; i < size; ++i) {
			byte = buf[i];
			byte = reverse(byte);
			for (j = 0u; j <= 7u; j++) {
				if ((int)(crc ^ byte) < 0) {
					crc = (crc << 1) ^ 0x04C11DB7;
				} else {
					crc = crc << 1;
				}
				byte = byte << 1;
			}
		}
		return reverse(~crc);
	}
}
