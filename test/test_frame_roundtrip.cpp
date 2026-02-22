#include <array>

#include <unity.h>

#include "nexbus/nexbus_frame.h"
#include "nexbus/nexbus_parser.h"

void test_frame_encode_parse_roundtrip() {
  const std::array<uint8_t, 4> payload{{0xDE, 0xAD, 0xBE, 0xEF}};
  nexbus::Frame tx{};
  tx.ver = nexbus::kVersion;
  tx.len = static_cast<uint16_t>(payload.size());
  tx.src = 0x10;
  tx.dst = 0x00;
  tx.msg = 0x20;
  tx.seq = 0x33;
  tx.flags = 0x08;
  tx.payload = {payload.data(), payload.size()};

  std::array<uint8_t, 64> encoded{};
  const size_t n = nexbus::encode_frame(tx, encoded.data(), encoded.size());
  TEST_ASSERT_GREATER_THAN_UINT32(0, static_cast<uint32_t>(n));

  nexbus::Parser parser;
  bool ready = false;
  for (size_t i = 0; i < n; ++i) {
    ready = parser.push(encoded[i]) || ready;
  }
  TEST_ASSERT_TRUE(ready);

  const nexbus::Frame& rx = parser.frame();
  TEST_ASSERT_EQUAL_UINT8(tx.ver, rx.ver);
  TEST_ASSERT_EQUAL_UINT16(tx.len, rx.len);
  TEST_ASSERT_EQUAL_UINT8(tx.src, rx.src);
  TEST_ASSERT_EQUAL_UINT8(tx.dst, rx.dst);
  TEST_ASSERT_EQUAL_UINT8(tx.msg, rx.msg);
  TEST_ASSERT_EQUAL_UINT8(tx.seq, rx.seq);
  TEST_ASSERT_EQUAL_UINT8(tx.flags, rx.flags);
  TEST_ASSERT_EQUAL_UINT32(payload.size(), rx.payload.size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(payload.data(), rx.payload.data, payload.size());
}
