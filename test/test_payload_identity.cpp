#include <array>

#include <unity.h>

#include "nexbus/nexbus_payloads.h"

void test_parse_identity_payload() {
  const std::array<uint8_t, 24> raw{{
      3, 'A', 'C', 'M',
      4, 'D', 'r', 'i', 'v',
      2, 'M', '1',
      4, 'S', 'N', '0', '1',
      7,
      1,
      9,
      0x78, 0x56, 0x34, 0x12,
  }};

  nexbus::IdentityPayload id{};
  const bool ok = nexbus::parse_identity_payload({raw.data(), raw.size()}, id);
  TEST_ASSERT_TRUE(ok);
  TEST_ASSERT_EQUAL_UINT32(3, id.vendor.size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(reinterpret_cast<const uint8_t*>("ACM"), id.vendor.data, 3);
  TEST_ASSERT_EQUAL_UINT8(7, id.hw_rev);
  TEST_ASSERT_EQUAL_UINT8(1, id.fw_major);
  TEST_ASSERT_EQUAL_UINT8(9, id.fw_minor);
  TEST_ASSERT_EQUAL_HEX32(0x12345678, id.caps_flags);
}
