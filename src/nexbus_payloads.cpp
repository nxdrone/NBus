#include "nexbus/nexbus_payloads.h"

namespace nexbus {
namespace {

bool read_u8(ByteView in, size_t& off, uint8_t& out) {
  if (off + 1 > in.size) {
    return false;
  }
  out = in.data[off++];
  return true;
}

bool read_u16_le(ByteView in, size_t& off, uint16_t& out) {
  if (off + 2 > in.size) {
    return false;
  }
  out = static_cast<uint16_t>(in.data[off]) |
        (static_cast<uint16_t>(in.data[off + 1]) << 8);
  off += 2;
  return true;
}

bool read_i16_le(ByteView in, size_t& off, int16_t& out) {
  uint16_t raw = 0;
  if (!read_u16_le(in, off, raw)) {
    return false;
  }
  out = static_cast<int16_t>(raw);
  return true;
}

bool read_u32_le(ByteView in, size_t& off, uint32_t& out) {
  if (off + 4 > in.size) {
    return false;
  }
  out = static_cast<uint32_t>(in.data[off]) |
        (static_cast<uint32_t>(in.data[off + 1]) << 8) |
        (static_cast<uint32_t>(in.data[off + 2]) << 16) |
        (static_cast<uint32_t>(in.data[off + 3]) << 24);
  off += 4;
  return true;
}

bool read_len_prefixed(ByteView in, size_t& off, ByteView& out) {
  uint8_t len = 0;
  if (!read_u8(in, off, len)) {
    return false;
  }
  if (off + len > in.size) {
    return false;
  }
  out = {in.data + off, len};
  off += len;
  return true;
}

}  // namespace

bool parse_identity_payload(ByteView payload, IdentityPayload& out) {
  size_t off = 0;
  if (!read_len_prefixed(payload, off, out.vendor) ||
      !read_len_prefixed(payload, off, out.product) ||
      !read_len_prefixed(payload, off, out.model) ||
      !read_len_prefixed(payload, off, out.serial) ||
      !read_u8(payload, off, out.hw_rev) ||
      !read_u8(payload, off, out.fw_major) ||
      !read_u8(payload, off, out.fw_minor) ||
      !read_u32_le(payload, off, out.caps_flags)) {
    return false;
  }
  return off == payload.size;
}

bool parse_caps_payload(ByteView payload, CapsPayload& out) {
  size_t off = 0;
  return read_u16_le(payload, off, out.power_aon_ma) &&
         read_u16_le(payload, off, out.power_vsw_ma) &&
         read_u16_le(payload, off, out.power_vsw_peak_ma) &&
         read_u8(payload, off, out.num_streams) &&
         read_u8(payload, off, out.num_params) &&
         read_u8(payload, off, out.num_outputs) &&
         read_u8(payload, off, out.reserved) && off == payload.size;
}

bool parse_streams_payload(ByteView payload, StreamsPayload& out) {
  size_t off = 0;
  if (!read_u8(payload, off, out.num_streams)) {
    return false;
  }
  if (out.num_streams > kMaxStreams) {
    return false;
  }

  for (size_t i = 0; i < out.num_streams; ++i) {
    auto& s = out.streams[i];
    if (!read_u8(payload, off, s.stream_id) ||
        !read_len_prefixed(payload, off, s.name) || s.name.size > 16 ||
        !read_u16_le(payload, off, s.rate_hz) ||
        !read_u32_le(payload, off, s.schema_id) ||
        !read_u16_le(payload, off, s.sample_len) ||
        !read_u16_le(payload, off, s.flags)) {
      return false;
    }
  }
  return off == payload.size;
}

bool parse_health_payload(ByteView payload, HealthPayload& out) {
  size_t off = 0;
  return read_u32_le(payload, off, out.time_ms) &&
         read_u16_le(payload, off, out.status_flags) &&
         read_u16_le(payload, off, out.v_aon_mv) &&
         read_u16_le(payload, off, out.v_vsw_mv) &&
         read_u16_le(payload, off, out.i_vsw_ma) &&
         read_i16_le(payload, off, out.temp_c_x10) &&
         read_u16_le(payload, off, out.reserved) && off == payload.size;
}

bool parse_event_payload(ByteView payload, EventPayload& out) {
  size_t off = 0;
  uint8_t data_len = 0;
  if (!read_u32_le(payload, off, out.time_ms) ||
      !read_u8(payload, off, out.event_class) ||
      !read_u16_le(payload, off, out.event_code) ||
      !read_u8(payload, off, out.severity) ||
      !read_u8(payload, off, data_len)) {
    return false;
  }
  if (off + data_len != payload.size) {
    return false;
  }
  out.data = {payload.data + off, data_len};
  return true;
}

}  // namespace nexbus
