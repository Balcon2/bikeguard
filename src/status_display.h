#pragma once

#include <Adafruit_SSD1306.h>

#include "imu.h"
#include "wheel_speed.h"

class StatusDisplay {
 public:
  StatusDisplay();
  bool begin(uint8_t address);
  void update(uint32_t now_ms, bool imu_ready, const WheelSpeed& wheel,
              const ImuSample& sample);

 private:
  Adafruit_SSD1306 display_;
  uint32_t last_update_ms_ = 0;
};
