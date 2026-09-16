#pragma once

#include <Arduino.h>
#include <Wire.h>

struct ImuSample {
  float accelX_g;
  float accelY_g;
  float accelZ_g;
  float gyroX_dps;
  float gyroY_dps;
  float gyroZ_dps;
};

class Mpu6050 {
 public:
  explicit Mpu6050(TwoWire& wire = Wire);

  bool begin(uint8_t address);
  bool read(ImuSample& sample);

 private:
  TwoWire& wire_;
  uint8_t address_ = 0;
};
