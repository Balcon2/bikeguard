#pragma once

#include <Arduino.h>

namespace config {

constexpr int I2C_SDA_PIN = 8;
constexpr int I2C_SCL_PIN = 9;
constexpr int HALL_PIN = 4;

constexpr uint8_t MPU6050_ADDRESS = 0x68;
constexpr uint8_t OLED_ADDRESS = 0x3C;

constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_PERIOD_MS = 20;
constexpr uint32_t DISPLAY_PERIOD_MS = 250;
constexpr uint32_t SPEED_PERIOD_MS = 250;

// Replace this with a measured wheel rollout in metres.
constexpr float WHEEL_CIRCUMFERENCE_M = 2.10f;

}  // namespace config
