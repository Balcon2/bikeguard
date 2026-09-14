#pragma once

#include <Arduino.h>

class WheelSpeed {
 public:
  void begin(uint8_t pin);
  void update(uint32_t now_ms, float wheel_circumference_m,
              uint32_t interval_ms);
  uint32_t pulseCount() const;
  float speedKmh() const;

  static WheelSpeed* activeInstance;
  static void IRAM_ATTR handleInterrupt();

 private:
  static constexpr uint32_t MIN_PULSE_INTERVAL_US = 2000;
  volatile uint32_t pulse_count_ = 0;
  volatile uint32_t last_pulse_us_ = 0;
  uint32_t last_update_ms_ = 0;
  uint32_t last_update_pulses_ = 0;
  float speed_kmh_ = 0.0f;
};
