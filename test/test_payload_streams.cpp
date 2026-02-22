#include <array>

#include <unity.h>

#include "nexbus/nexbus_payloads.h"
#include "nexbus/nexbus_schemas.h"

void test_parse_streams_payload() {
  const std::array<uint8_t, 31> raw{{
      2,
      1, 5, 'd', 'r', 'i', 'v', 'e', 100, 0,
      0x01, 0x10, 0x00, 0x00,
      12, 0,
      1, 0,
      2, 5, 'p', 'o', 'w', 'e', 'r', 50, 0,
      0x02, 0x10, 0x00, 0x00,
      0, 0,
      3, 0,
  }};

  nexbus::StreamsPayload streams{};
  const bool ok = nexbus::parse_streams_payload({raw.data(), raw.size()}, streams);
  TEST_ASSERT_TRUE(ok);
  TEST_ASSERT_EQUAL_UINT8(2, streams.num_streams);
  TEST_ASSERT_EQUAL_UINT8(1, streams.streams[0].stream_id);
  TEST_ASSERT_EQUAL_UINT32(nexbus::schema::DRIVE_TELEM_V1, streams.streams[0].schema_id);
  TEST_ASSERT_EQUAL_UINT16(12, streams.streams[0].sample_len);
  TEST_ASSERT_EQUAL_UINT8(2, streams.streams[1].stream_id);
  TEST_ASSERT_EQUAL_UINT32(nexbus::schema::POWER_TELEM_V1, streams.streams[1].schema_id);
  TEST_ASSERT_EQUAL_UINT16(0, streams.streams[1].sample_len);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_parse_streams_payload);
  return UNITY_END();
}
