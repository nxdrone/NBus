#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "nexbus/nexbus_types.h"

namespace nexbus {

struct IdentityPayload {
  ByteView vendor;
  ByteView product;
  ByteView model;
  ByteView serial;
  uint8_t hw_rev = 0;
  uint8_t fw_major = 0;
  uint8_t fw_minor = 0;
  uint32_t caps_flags = 0;
};

struct CapsPayload {
  uint16_t power_aon_ma = 0;
  uint16_t power_vsw_ma = 0;
  uint16_t power_vsw_peak_ma = 0;
  uint8_t num_streams = 0;
  uint8_t num_params = 0;
  uint8_t num_outputs = 0;
  uint8_t reserved = 0;
};

struct StreamDesc {
  uint8_t stream_id = 0;
  ByteView name;
  uint16_t rate_hz = 0;
  uint32_t schema_id = 0;
  uint16_t sample_len = 0;
  uint16_t flags = 0;
};

constexpr size_t kMaxStreams = 16;

struct StreamsPayload {
  uint8_t num_streams = 0;
  std::array<StreamDesc, kMaxStreams> streams{};
};

struct HealthPayload {
  uint32_t time_ms = 0;
  uint16_t status_flags = 0;
  uint16_t v_aon_mv = 0;
  uint16_t v_vsw_mv = 0;
  uint16_t i_vsw_ma = 0;
  int16_t temp_c_x10 = 0;
  uint16_t reserved = 0;
};

struct EventPayload {
  uint32_t time_ms = 0;
  uint8_t event_class = 0;
  uint16_t event_code = 0;
  uint8_t severity = 0;
  ByteView data;
};

bool parse_identity_payload(ByteView payload, IdentityPayload& out);
bool parse_caps_payload(ByteView payload, CapsPayload& out);
bool parse_streams_payload(ByteView payload, StreamsPayload& out);
bool parse_health_payload(ByteView payload, HealthPayload& out);
bool parse_event_payload(ByteView payload, EventPayload& out);

}  // namespace nexbus
