#include <Arduino.h>
#include <Wire.h>

#include "config.h"
#include "csv_logger.h"
#include "imu.h"
#include "status_display.h"
#include "wheel_speed.h"

Mpu6050 imu;
WheelSpeed wheel;
CsvLogger logger;
StatusDisplay display;
ImuSample latest_sample{};
bool imu_ready = false;
uint32_t last_sample_ms = 0;

void setup() {
  Serial.begin(config::SERIAL_BAUD);
  delay(300);

  Wire.begin(config::I2C_SDA_PIN, config::I2C_SCL_PIN);
  wheel.begin(config::HALL_PIN);
  display.begin(config::OLED_ADDRESS);
  imu_ready = imu.begin(config::MPU6050_ADDRESS);
  logger.begin();

  if (!imu_ready) {
    Serial.println("# ERROR: MPU6050 was not detected; CSV samples are paused.");
  }
}

void loop() {
  const uint32_t now_ms = millis();
  wheel.update(now_ms, config::WHEEL_CIRCUMFERENCE_M,
               config::SPEED_PERIOD_MS);
  display.update(now_ms, imu_ready, wheel, latest_sample);

  if (now_ms - last_sample_ms < config::SAMPLE_PERIOD_MS) {
    return;
  }
  last_sample_ms = now_ms;

  if (!imu_ready || !imu.read(latest_sample)) {
    return;
  }
  logger.write(now_ms, latest_sample, wheel);
}
