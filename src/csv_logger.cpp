#include "csv_logger.h"

void CsvLogger::begin() {
  Serial.println("timestamp_ms,accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,"
                 "gyro_z_dps,wheel_pulses,speed_kmh,event_label");
}

void CsvLogger::write(uint32_t timestamp_ms, const ImuSample& sample,
                      const WheelSpeed& wheel) {
  Serial.print(timestamp_ms);
  Serial.print(',');
  Serial.print(sample.accelX_g, 5);
  Serial.print(',');
  Serial.print(sample.accelY_g, 5);
  Serial.print(',');
  Serial.print(sample.accelZ_g, 5);
  Serial.print(',');
  Serial.print(sample.gyroX_dps, 5);
  Serial.print(',');
  Serial.print(sample.gyroY_dps, 5);
  Serial.print(',');
  Serial.print(sample.gyroZ_dps, 5);
  Serial.print(',');
  Serial.print(wheel.pulseCount());
  Serial.print(',');
  Serial.print(wheel.speedKmh(), 3);
  // Labels are assigned during later dataset annotation, never guessed on-device.
  Serial.println(',');
}
