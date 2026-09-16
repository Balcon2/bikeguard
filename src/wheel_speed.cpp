#include "wheel_speed.h"

WheelSpeed* WheelSpeed::activeInstance = nullptr;

void WheelSpeed::begin(uint8_t pin) {
  activeInstance = this;
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt, FALLING);
  last_update_ms_ = millis();
}

void IRAM_ATTR WheelSpeed::handleInterrupt() {
  if (activeInstance == nullptr) {
    return;
  }
  const uint32_t now_us = micros();
  if (now_us - activeInstance->last_pulse_us_ >= MIN_PULSE_INTERVAL_US) {
    ++activeInstance->pulse_count_;
    activeInstance->last_pulse_us_ = now_us;
  }
}

void WheelSpeed::update(uint32_t now_ms, float wheel_circumference_m,
                        uint32_t interval_ms) {
  if (now_ms - last_update_ms_ < interval_ms) {
    return;
  }

  noInterrupts();
  const uint32_t pulses = pulse_count_;
  interrupts();
  const uint32_t new_pulses = pulses - last_update_pulses_;
  const uint32_t elapsed_ms = now_ms - last_update_ms_;
  speed_kmh_ = (new_pulses * wheel_circumference_m * 3600.0f) / elapsed_ms;
  last_update_pulses_ = pulses;
  last_update_ms_ = now_ms;
}

uint32_t WheelSpeed::pulseCount() const {
  noInterrupts();
  const uint32_t count = pulse_count_;
  interrupts();
  return count;
}

float WheelSpeed::speedKmh() const { return speed_kmh_; }
