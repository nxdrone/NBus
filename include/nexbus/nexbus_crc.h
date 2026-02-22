#pragma once

#include <cstddef>
#include <cstdint>

namespace nexbus {
uint16_t crc16_ccitt_false(const uint8_t* data, size_t len);
}  // namespace nexbus
