#pragma once

#include <cstddef>
#include <cstdint>

#include "nexbus/nexbus_types.h"

namespace nexbus {

size_t encoded_frame_size(const Frame& frame);

// Encodes into caller-provided output buffer. Returns bytes written, or 0 on error.
size_t encode_frame(const Frame& frame, uint8_t* out, size_t out_capacity);

}  // namespace nexbus
