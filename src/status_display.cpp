#include "status_display.h"

#include "config.h"

StatusDisplay::StatusDisplay() : display_(128, 64, &Wire, -1) {}

bool StatusDisplay::begin(uint8_t address) {
  if (!display_.begin(SSD1306_SWITCHCAPVCC, address)) {
    return false;
  }
  display_.clearDisplay();
  display_.setTextColor(SSD1306_WHITE);
  display_.setTextSize(1);
  display_.setCursor(0, 0);
  display_.println("Starting BikeGuard");
  display_.display();
  return true;
}

void StatusDisplay::update(uint32_t now_ms, bool imu_ready,
                           const WheelSpeed& wheel, const ImuSample& sample) {
  if (now_ms - last_update_ms_ < config::DISPLAY_PERIOD_MS) {
    return;
  }
  last_update_ms_ = now_ms;
  display_.clearDisplay();
  display_.setCursor(0, 0);
  display_.println(imu_ready ? "BIKEGUARD LOGGER" : "IMU NOT FOUND");
  display_.setCursor(0, 16);
  display_.print("Speed: ");
  display_.print(wheel.speedKmh(), 1);
  display_.println(" km/h");
  display_.setCursor(0, 30);
  display_.print("Pulses: ");
  display_.println(wheel.pulseCount());
  display_.setCursor(0, 44);
  display_.print("Az: ");
  display_.print(sample.accelZ_g, 2);
  display_.println(" g");
  display_.display();
}
