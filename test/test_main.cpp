#include <unity.h>

void setUp() {}
void tearDown() {}

void test_crc_known_vector();
void test_frame_encode_parse_roundtrip();
void test_parser_resync_after_garbage();
void test_parse_identity_payload();
void test_parse_streams_payload();

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_crc_known_vector);
  RUN_TEST(test_frame_encode_parse_roundtrip);
  RUN_TEST(test_parser_resync_after_garbage);
  RUN_TEST(test_parse_identity_payload);
  RUN_TEST(test_parse_streams_payload);
  return UNITY_END();
}
