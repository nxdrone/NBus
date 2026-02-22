#include <unity.h>

#include "nexbus/nexbus_crc.h"

void test_crc_known_vector() {
  const char* text = "123456789";
  const uint16_t crc = nexbus::crc16_ccitt_false(
      reinterpret_cast<const uint8_t*>(text), 9);
  TEST_ASSERT_EQUAL_HEX16(0x29B1, crc);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_crc_known_vector);
  return UNITY_END();
}
