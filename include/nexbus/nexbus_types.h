#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace nexbus {

struct ByteView {
  const uint8_t* data = nullptr;
  size_t size = 0;
};

constexpr uint8_t kSof = 0xA5;
constexpr uint8_t kVersion = 0x01;
constexpr size_t kHeaderSizeNoSof = 8;  // VER+LEN+SRC+DST+MSG+SEQ+FLAGS
constexpr size_t kCrcSize = 2;
constexpr size_t kMinFrameSize = 1 + kHeaderSizeNoSof + kCrcSize;
constexpr size_t kDefaultMaxPayloadLen = 1024;

struct Frame {
  uint8_t ver = kVersion;
  uint16_t len = 0;
  uint8_t src = 0;
  uint8_t dst = 0;
  uint8_t msg = 0;
  uint8_t seq = 0;
  uint8_t flags = 0;
  ByteView payload{};
};

}  // namespace nexbus
