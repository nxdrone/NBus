#include "nexbus/nexbus_frame.h"

#include "nexbus/nexbus_crc.h"

namespace nexbus {

size_t encoded_frame_size(const Frame& frame) {
  return kMinFrameSize + frame.payload.size;
}

size_t encode_frame(const Frame& frame, uint8_t* out, size_t out_capacity) {
  if (out == nullptr) {
    return 0;
  }
  if (frame.len != frame.payload.size) {
    return 0;
  }

  const size_t required = encoded_frame_size(frame);
  if (out_capacity < required) {
    return 0;
  }

  size_t idx = 0;
  out[idx++] = kSof;
  out[idx++] = frame.ver;
  out[idx++] = static_cast<uint8_t>(frame.len & 0xFFu);
  out[idx++] = static_cast<uint8_t>((frame.len >> 8) & 0xFFu);
  out[idx++] = frame.src;
  out[idx++] = frame.dst;
  out[idx++] = frame.msg;
  out[idx++] = frame.seq;
  out[idx++] = frame.flags;

  for (size_t i = 0; i < frame.payload.size; ++i) {
    out[idx++] = frame.payload.data[i];
  }

  const uint16_t crc = crc16_ccitt_false(&out[1], kHeaderSizeNoSof + frame.payload.size);
  out[idx++] = static_cast<uint8_t>(crc & 0xFFu);
  out[idx++] = static_cast<uint8_t>((crc >> 8) & 0xFFu);
  return idx;
}

}  // namespace nexbus
