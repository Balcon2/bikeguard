## Wiring

### Pin Connections

| Component                             | Pin           | ESP32-S3 Board Pin  | Function                |
| ------------------------------------- | ------------- | ------------------- | ----------------------- |
| **Bosch BMI270 IMU**                  | VCC           | 3V3                 | Power                   |
| **Bosch BMI270 IMU**                  | GND           | GND                 | Ground                  |
| **Bosch BMI270 IMU**                  | SDA           | GPIO8               | I²C Data                |
| **Bosch BMI270 IMU**                  | SCL           | GPIO9               | I²C Clock               |
| **SSD1306 OLED**                      | VCC           | 3V3                 | Power                   |
| **SSD1306 OLED**                      | GND           | GND                 | Ground                  |
| **SSD1306 OLED**                      | SDA           | GPIO8               | I²C Data                |
| **SSD1306 OLED**                      | SCL           | GPIO9               | I²C Clock               |
| **M5Stack Hall Effect Unit (A3144E)** | 5V/VCC        | 3V3*                | Power                   |
| **M5Stack Hall Effect Unit (A3144E)** | GND           | GND                 | Ground                  |
| **M5Stack Hall Effect Unit (A3144E)** | OUT           | GPIO4               | Digital Signal          |
| **Neodymium Disc Magnet 10 × 3 mm**   | Magnetic face | Bicycle wheel spoke | Trigger for Hall sensor |

### I²C Bus

The **BMI270** and **SSD1306 OLED** share the same I²C bus:

```text
ESP32-S3
├── GPIO8 (SDA) ── BMI270 SDA
│               └── OLED SDA
│
└── GPIO9 (SCL) ── BMI270 SCL
                └── OLED SCL
```

This is normal because I²C allows multiple devices to share the same SDA and SCL lines, as long as they use different I²C addresses.

### Hall Sensor

The magnet is attached to a bicycle wheel spoke. The Hall sensor is fixed to the bicycle frame so that the magnet passes close to it once per wheel revolution.

```text
   Wheel
    │
    │   ● Magnet
    │       ↓
    │     [Hall Sensor]
    │          │
    └──────────┴── Bicycle frame
```

Each detected magnetic pulse represents one wheel revolution and is used to estimate wheel speed.

> **Note:** Verify the exact power-input specification of your Hall Effect Unit before connecting it. Some Hall modules are designed for 5 V operation, while the ESP32 GPIO operates at 3.3 V logic. The sensor output must be safe for the ESP32 GPIO.
