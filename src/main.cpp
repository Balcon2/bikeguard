#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Forward declarations
void IRAM_ATTR onWheelPulse();
void updateSpeed(uint32_t now);
void drawStatus(uint32_t now);

constexpr int I2C_SDA_PIN = 8;
constexpr int I2C_SCL_PIN = 9;
constexpr int HALL_PIN = 4;
constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr uint32_t SAMPLE_PERIOD_MS = 20;  // 50 samples per second
constexpr float WHEEL_CIRCUMFERENCE_M = 2.10f; // Measure your own tyre rollout later.

BMI270 imu;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
volatile uint32_t wheelPulses = 0;
uint32_t lastSampleMs = 0;
uint32_t lastDisplayMs = 0;
uint32_t lastSpeedPulseCount = 0;
uint32_t lastSpeedMs = 0;
float speedKmh = 0.0f;
bool imuReady = false;

void IRAM_ATTR onWheelPulse() {
  wheelPulses++;
}

void updateSpeed(uint32_t now) {
  if (now - lastSpeedMs < 1000) return;
  noInterrupts();
  const uint32_t pulseSnapshot = wheelPulses;
  interrupts();
  const uint32_t newPulses = pulseSnapshot - lastSpeedPulseCount;
  const uint32_t elapsedMs = now - lastSpeedMs;
  speedKmh = (newPulses * WHEEL_CIRCUMFERENCE_M * 3600.0f) / elapsedMs;
  lastSpeedPulseCount = pulseSnapshot;
  lastSpeedMs = now;
}

void drawStatus(uint32_t now) {
  if (now - lastDisplayMs < 250) return;
  lastDisplayMs = now;
  noInterrupts();
  const uint32_t pulseSnapshot = wheelPulses;
  interrupts();
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(imuReady ? "BIKE LOGGER" : "IMU NOT FOUND");
  display.setCursor(0, 18);
  display.print("Speed: ");
  display.print(speedKmh, 1);
  display.println(" km/h");
  display.setCursor(0, 34);
  display.print("Wheel pulses: ");
  display.println(pulseSnapshot);
  display.setCursor(0, 50);
  display.println("USB CSV: 50 Hz");
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(400);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), onWheelPulse, FALLING);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Starting logger...");
  display.display();

  imuReady = imu.beginI2C();
  lastSpeedMs = millis();
  Serial.println("time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,wheel_pulses,speed_kmh");
}

void loop() {
  const uint32_t now = millis();
  updateSpeed(now);
  drawStatus(now);
  if (now - lastSampleMs < SAMPLE_PERIOD_MS) return;
  lastSampleMs = now;

  if (!imuReady) return;
  if (imu.getSensorData() != BMI2_OK) return;

  noInterrupts();
  const uint32_t pulseSnapshot = wheelPulses;
  interrupts();

  Serial.print(now);
  Serial.print(',');
  Serial.print(imu.data.accelX, 5);
  Serial.print(',');
  Serial.print(imu.data.accelY, 5);
  Serial.print(',');
  Serial.print(imu.data.accelZ, 5);
  Serial.print(',');
  Serial.print(imu.data.gyroX, 5);
  Serial.print(',');
  Serial.print(imu.data.gyroY, 5);
  Serial.print(',');
  Serial.print(imu.data.gyroZ, 5);
  Serial.print(',');
  Serial.print(pulseSnapshot);
  Serial.print(',');
  Serial.println(speedKmh, 3);
}
