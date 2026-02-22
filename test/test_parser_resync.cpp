#include <array>

#include <unity.h>

#include "nexbus/nexbus_frame.h"
#include "nexbus/nexbus_parser.h"

void test_parser_resync_after_garbage() {
  const std::array<uint8_t, 2> payload{{0xAA, 0x55}};
  nexbus::Frame tx{};
  tx.ver = nexbus::kVersion;
  tx.len = static_cast<uint16_t>(payload.size());
  tx.src = 0x01;
  tx.dst = 0x02;
  tx.msg = 0x03;
  tx.seq = 0x04;
  tx.flags = 0x00;
  tx.payload = {payload.data(), payload.size()};

  std::array<uint8_t, 32> encoded{};
  const size_t n = nexbus::encode_frame(tx, encoded.data(), encoded.size());
  TEST_ASSERT_GREATER_THAN_UINT32(0, static_cast<uint32_t>(n));

  nexbus::Parser parser;
  std::array<uint8_t, 5> garbage{{0x00, 0xFF, 0x11, 0x22, 0x33}};
  for (uint8_t b : garbage) {
    TEST_ASSERT_FALSE(parser.push(b));
  }

  bool ready = false;
  for (size_t i = 0; i < n; ++i) {
    ready = parser.push(encoded[i]) || ready;
  }

  TEST_ASSERT_TRUE(ready);
  const nexbus::Frame& rx = parser.frame();
  TEST_ASSERT_EQUAL_UINT8(tx.msg, rx.msg);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(payload.data(), rx.payload.data, payload.size());
}
