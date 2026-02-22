#include "nexbus/nexbus_parser.h"

#include "nexbus/nexbus_crc.h"

namespace nexbus {

Parser::Parser(size_t max_payload_len) : max_payload_len_(max_payload_len) {
  body_.resize(kHeaderSizeNoSof + max_payload_len_);
  payload_.resize(max_payload_len_);
  reset_to_wait_sof();
}

void Parser::reset_to_wait_sof() {
  state_ = State::WaitSof;
  needed_ = 0;
  index_ = 0;
}

void Parser::reset() { reset_to_wait_sof(); }

const Frame& Parser::frame() const { return last_frame_; }

bool Parser::push(uint8_t byte) {
  switch (state_) {
    case State::WaitSof:
      if (byte == kSof) {
        state_ = State::ReadHeader;
        needed_ = kHeaderSizeNoSof;
        index_ = 0;
      }
      return false;

    case State::ReadHeader:
      body_[index_++] = byte;
      if (index_ < needed_) {
        return false;
      }

      if (body_[0] != kVersion) {
        reset_to_wait_sof();
        if (byte == kSof) {
          state_ = State::ReadHeader;
          needed_ = kHeaderSizeNoSof;
          index_ = 0;
        }
        return false;
      }

      {
        const uint16_t len = static_cast<uint16_t>(body_[1]) |
                             (static_cast<uint16_t>(body_[2]) << 8);
        if (len > max_payload_len_) {
          reset_to_wait_sof();
          return false;
        }

        if (len == 0) {
          state_ = State::ReadCrc;
          needed_ = 2;
          index_ = 0;
        } else {
          state_ = State::ReadPayload;
          needed_ = len;
          index_ = 0;
        }
      }
      return false;

    case State::ReadPayload:
      body_[kHeaderSizeNoSof + index_] = byte;
      payload_[index_] = byte;
      ++index_;
      if (index_ >= needed_) {
        state_ = State::ReadCrc;
        needed_ = 2;
        index_ = 0;
      }
      return false;

    case State::ReadCrc:
      crc_bytes_[index_++] = byte;
      if (index_ < needed_) {
        return false;
      }

      {
        const uint16_t len = static_cast<uint16_t>(body_[1]) |
                             (static_cast<uint16_t>(body_[2]) << 8);
        const size_t body_len = kHeaderSizeNoSof + len;
        const uint16_t got_crc = static_cast<uint16_t>(crc_bytes_[0]) |
                                 (static_cast<uint16_t>(crc_bytes_[1]) << 8);
        const uint16_t calc_crc = crc16_ccitt_false(body_.data(), body_len);

        reset_to_wait_sof();
        if (got_crc != calc_crc) {
          return false;
        }

        last_frame_.ver = body_[0];
        last_frame_.len = len;
        last_frame_.src = body_[3];
        last_frame_.dst = body_[4];
        last_frame_.msg = body_[5];
        last_frame_.seq = body_[6];
        last_frame_.flags = body_[7];
        last_frame_.payload = {payload_.data(), len};
        return true;
      }
  }

  reset_to_wait_sof();
  return false;
}

}  // namespace nexbus
