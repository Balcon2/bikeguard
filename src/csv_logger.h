#pragma once

#include "imu.h"
#include "wheel_speed.h"

class CsvLogger {
 public:
  void begin();
  void write(uint32_t timestamp_ms, const ImuSample& sample,
             const WheelSpeed& wheel);
};
