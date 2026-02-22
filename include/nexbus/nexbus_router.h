#pragma once

#include <cstdint>

#include "nexbus/nexbus_types.h"

namespace nexbus {

class IFrameHandler {
 public:
  virtual ~IFrameHandler() = default;
  virtual bool handle(const Frame& frame) = 0;
};

class Router {
 public:
  explicit Router(IFrameHandler& handler) : handler_(handler) {}
  bool dispatch(const Frame& frame) { return handler_.handle(frame); }

 private:
  IFrameHandler& handler_;
};

}  // namespace nexbus
