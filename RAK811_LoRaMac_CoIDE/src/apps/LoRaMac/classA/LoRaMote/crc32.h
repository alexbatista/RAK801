#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

uint32_t crc32(const uint8_t *buf, uint16_t size, uint32_t crc);

#endif //CRC32_H