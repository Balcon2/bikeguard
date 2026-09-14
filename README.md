# BikeGuard

BikeGuard is an open-source hardware + machine-learning research project exploring a bicycle/e-bike crash-detection and reusable safety-trigger system, built around an ESP32-S3.

> **Status: early-stage prototype / research repo.** No finished firmware, ML model, or dataset exists yet. This README describes the current plan and the parts being sourced for the first hardware bring-up stage, not a completed product.

## Concept

```
Sensors -> ESP32-S3 -> Motion + wheel-speed data -> Data processing / ML -> Crash detection -> Safety trigger
```

The system is meant to continuously monitor bicycle motion and wheel speed, eventually using an on-device (TinyML) model to tell normal riding apart from a real crash, and trigger a reusable safety mechanism when it detects one. It is **not** a certified safety device and must never be treated as reliable crash protection.

## Prototype parts list

Selected for being cheap, beginner-friendly, and usable from both C++ (Arduino) and Python (MicroPython) — a full sensor + board set fits in roughly **EUR 20-35**.

| Part | Role | Why this part |
|---|---|---|
| ESP32-S3 dev board | Main microcontroller; reads all sensors, runs firmware, later runs TinyML inference | Dual-core, MicroPython + Arduino support, enough RAM/flash for future on-device ML |
| MPU6050 (GY-521 breakout) | 6-axis accelerometer + gyroscope for motion/crash sensing | Extremely common, cheap (~EUR 3-5), simple I2C interface, many ready-made MicroPython libraries — easiest IMU to learn on |
| Hall-effect sensor module (A3144/LM393) + magnet | Wheel-speed measurement | Simple digital on/off signal, near EUR 1-3, trivial to wire and read |
| SSD1306 OLED (I2C, 0.96") | Local debug/status display | Cheap, well-supported MicroPython driver, useful for checking sensor values without a laptop |

Supporting prototyping supplies (reusable across future projects): a breadboard, an assorted jumper-wire pack, and a USB cable matching the board's port (USB-C or micro-USB, depending on the exact board).

Note: an earlier direction considered the Bosch BMI270 IMU. It's a better chip long-term (lower power, better noise performance for a final wearable/on-bike unit), but it is more expensive, has weaker beginner/MicroPython support, and is harder to source cheaply in the EU. The MPU6050 is used for this prototyping stage instead; BMI270 stays a candidate for a later, more refined hardware revision.

## Current development stage

Following the project roadmap, work right now is focused on:

1. Hardware and sensor bring-up (in progress)
2. Reliable sensor logging (planned)
3. Dataset creation (planned)
4. Signal analysis in Python (planned)
5. Rule-based crash-detection baseline (planned)
6. ML experimentation (planned, only after real data exists)
7. TinyML optimization and deployment (planned)
8. Real-time embedded integration (planned)
9. Safe simulated deployment testing, e.g. LEDs/buzzer/OLED messages instead of a real actuator (planned)
10. Hardware refinement and documentation (planned)
## Firmware prototype

The first firmware prototype is in `src/` and is built with PlatformIO:

```text
src/
  config.h           # pins, I2C addresses, and sampling intervals
  imu.*              # MPU6050 bring-up and raw sample conversion
  wheel_speed.*      # interrupt-driven Hall pulse counting and speed
  status_display.*   # SSD1306 debug/status output
  csv_logger.*       # synchronized CSV output over USB serial
  main.cpp           # non-blocking application loop
```

Build and upload with:

```bash
pio run
pio run --target upload
pio device monitor
```

The firmware emits the planned sensor columns as CSV at 50 Hz when the MPU6050
is detected. Pin assignments and the measured wheel circumference must be
updated in `src/config.h` for the actual wiring. A missing IMU is reported over
serial and does not produce fabricated samples.

## Software

- **Firmware**: C++ (Arduino) and/or MicroPython on the ESP32-S3, kept modular (separate sensor drivers, logging, display, configuration).
- **Data/ML tooling**: Python (pandas, numpy, matplotlib, and later a TinyML framework) for cleaning, visualizing, and eventually training on sensor data.

See [AGENTS.md](AGENTS.md) for full technical conventions, coding guidelines, and safety rules for contributors (including AI coding agents).

## Safety

BikeGuard is an experimental research prototype, not a certified safety device. Data collection must never involve real, dangerous crashes — only normal riding plus safe, controlled events (braking, bumps, low-speed controlled falls). Any future safety-trigger/actuator work starts with safe stand-ins (LEDs, buzzer, logging) before anything resembling a real deployment mechanism, which is treated as a separate, later, safety-critical engineering problem.
