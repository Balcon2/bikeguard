#include "imu.h"

namespace {
constexpr uint8_t REG_WHO_AM_I = 0x75;
constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
constexpr uint8_t REG_ACCEL_CONFIG = 0x1C;
constexpr uint8_t REG_GYRO_CONFIG = 0x1B;
constexpr uint8_t REG_ACCEL_XOUT_H = 0x3B;
constexpr uint8_t EXPECTED_WHO_AM_I = 0x68;
constexpr float ACCEL_SCALE_LSB_PER_G = 16384.0f;  // +/-2 g
constexpr float GYRO_SCALE_LSB_PER_DPS = 131.0f;   // +/-250 dps

bool writeRegister(TwoWire& wire, uint8_t address, uint8_t reg, uint8_t value) {
  wire.beginTransmission(address);
  wire.write(reg);
  wire.write(value);
  return wire.endTransmission() == 0;
}

bool readRegisters(TwoWire& wire, uint8_t address, uint8_t reg, uint8_t* data,
                   size_t length) {
  wire.beginTransmission(address);
  wire.write(reg);
  if (wire.endTransmission(false) != 0 || wire.requestFrom(address, length) != length) {
    return false;
  }
  for (size_t i = 0; i < length; ++i) {
    data[i] = wire.read();
  }
  return true;
}

int16_t toInt16(const uint8_t high, const uint8_t low) {
  return static_cast<int16_t>((static_cast<uint16_t>(high) << 8) | low);
}
}  // namespace

Mpu6050::Mpu6050(TwoWire& wire) : wire_(wire) {}

bool Mpu6050::begin(uint8_t address) {
  address_ = address;
  uint8_t identity = 0;
  if (!readRegisters(wire_, address_, REG_WHO_AM_I, &identity, 1) ||
      identity != EXPECTED_WHO_AM_I) {
    return false;
  }

  return writeRegister(wire_, address_, REG_PWR_MGMT_1, 0x00) &&
         writeRegister(wire_, address_, REG_ACCEL_CONFIG, 0x00) &&
         writeRegister(wire_, address_, REG_GYRO_CONFIG, 0x00);
}

bool Mpu6050::read(ImuSample& sample) {
  uint8_t raw[14] = {};
  if (!readRegisters(wire_, address_, REG_ACCEL_XOUT_H, raw, sizeof(raw))) {
    return false;
  }

  sample.accelX_g = toInt16(raw[0], raw[1]) / ACCEL_SCALE_LSB_PER_G;
  sample.accelY_g = toInt16(raw[2], raw[3]) / ACCEL_SCALE_LSB_PER_G;
  sample.accelZ_g = toInt16(raw[4], raw[5]) / ACCEL_SCALE_LSB_PER_G;
  sample.gyroX_dps = toInt16(raw[8], raw[9]) / GYRO_SCALE_LSB_PER_DPS;
  sample.gyroY_dps = toInt16(raw[10], raw[11]) / GYRO_SCALE_LSB_PER_DPS;
  sample.gyroZ_dps = toInt16(raw[12], raw[13]) / GYRO_SCALE_LSB_PER_DPS;
  return true;
}
