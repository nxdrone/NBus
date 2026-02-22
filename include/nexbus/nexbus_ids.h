#pragma once

#include <cstdint>

namespace nexbus {
namespace msg {
constexpr uint8_t PING = 0x01;
constexpr uint8_t PONG = 0x02;
constexpr uint8_t GET_IDENTITY = 0x03;
constexpr uint8_t IDENTITY = 0x04;
constexpr uint8_t GET_CAPS = 0x05;
constexpr uint8_t CAPS = 0x06;
constexpr uint8_t GET_STREAMS = 0x07;
constexpr uint8_t STREAMS = 0x08;
constexpr uint8_t SET_NODE_ID = 0x09;
constexpr uint8_t ACK = 0x0A;
constexpr uint8_t NACK = 0x0B;
constexpr uint8_t TIME_SYNC = 0x10;
constexpr uint8_t TELEMETRY_SAMPLE = 0x20;
constexpr uint8_t EVENT = 0x21;
constexpr uint8_t HEALTH = 0x22;
constexpr uint8_t DEBUG_TEXT = 0x23;
}  // namespace msg

namespace flag {
constexpr uint8_t ACK_REQ = 1u << 0;
constexpr uint8_t IS_ACK = 1u << 1;
constexpr uint8_t IS_NACK = 1u << 2;
constexpr uint8_t STREAM = 1u << 3;
constexpr uint8_t EVENT = 1u << 4;
}  // namespace flag

constexpr uint8_t kMasterNodeId = 0x00;
constexpr uint8_t kBroadcastNodeId = 0xFF;
}  // namespace nexbus
