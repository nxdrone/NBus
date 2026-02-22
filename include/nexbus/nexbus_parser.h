#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "nexbus/nexbus_types.h"

namespace nexbus {

class Parser {
 public:
  explicit Parser(size_t max_payload_len = kDefaultMaxPayloadLen);

  bool push(uint8_t byte);
  const Frame& frame() const;
  void reset();

 private:
  enum class State : uint8_t {
    WaitSof,
    ReadHeader,
    ReadPayload,
    ReadCrc,
  };

  void reset_to_wait_sof();

  State state_ = State::WaitSof;
  size_t max_payload_len_ = kDefaultMaxPayloadLen;
  std::vector<uint8_t> body_;    // VER..PAYLOAD
  std::vector<uint8_t> payload_; // payload storage for frame view
  uint8_t crc_bytes_[2]{};
  size_t needed_ = 0;
  size_t index_ = 0;
  Frame last_frame_{};
};

}  // namespace nexbus
