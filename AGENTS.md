# AGENTS.md

This file gives AI coding agents (and humans) the context needed to work on **BikeGuard** correctly.

## Project status: early-stage research repo

BikeGuard is a personal open-source hardware + ML project to research a bicycle/e-bike crash-detection and airbag system.

**Nothing is implemented yet.** As of now this repo contains only documentation (README). There is no firmware, no ML code, no dataset, and no hardware build. Do not assume any of these exist. Do not generate code that pretends prior stages are done.

Treat every contribution as part of an incremental, staged research project — not a finished product.

## What BikeGuard is

Concept pipeline:

```
Sensors -> ESP32-S3 -> Motion + wheel-speed data -> Data processing / ML -> Crash detection -> Safety/airbag system
```

Goal: a lightweight, affordable, reusable, open-source crash-detection system with local real-time processing on an ESP32-S3, eventually triggering a reusable safety mechanism (not necessarily a pyrotechnic airbag).

Planned hardware (not yet integrated):
- ESP32-S3 (main microcontroller)
- Bosch BMI270 IMU (accel + gyro)
- SSD1306 OLED (debug/status display)
- Hall-effect sensor + wheel magnet (wheel speed)

## Roadmap (follow this order)

1. Hardware and sensor bring-up
2. Reliable sensor logging
3. Dataset creation
4. Signal analysis in Python
5. Rule-based baseline
6. ML experimentation
7. TinyML optimization/deployment
8. Real-time embedded integration
9. Safe simulated deployment testing
10. Hardware refinement and documentation

Current focus is stages 1–2: get the BMI270, Hall sensor, and OLED individually verified and logging synchronized data from the ESP32-S3. Do not jump ahead to ML or TinyML work before real sensor data exists.

## Tech stack and conventions

**Embedded firmware**
- Language: C++ (Arduino framework) targeting ESP32-S3
- Keep modules small and separated by responsibility, e.g.:
  - BMI270 driver
  - Hall sensor / wheel-speed measurement
  - OLED/debug output
  - Sensor sampling
  - Timestamping
  - Data logging
  - Configuration/calibration
- Do not put everything in one large `.ino` file
- Keep pins and configuration easily changeable (e.g. a `config.h`)
- Keep the main loop predictable and non-blocking; avoid heavy work or long delays in the real-time loop
- Avoid unnecessary libraries/dependencies; favor lightweight, well-maintained ones suited to ESP32-S3 resource limits (RAM, flash, CPU, power)

**Data / ML tooling**
- Language: Python, for data processing, visualization, feature engineering, and model training
- Prefer pandas/numpy for data handling and matplotlib for visualization unless there's a specific reason otherwise
- Eventual on-device inference should target TinyML (e.g. TensorFlow Lite Micro) constraints — small model size, low latency, low memory
- No dataset or trained model currently exists in this repo; do not fabricate sample data, accuracy numbers, or results

**Dataset format (planned, may evolve)**

```
timestamp, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, wheel_speed, event_label
```

Document sensor orientation, coordinate conventions, and calibration wherever they affect data or ML — these details materially affect model quality.

## Working rules for agents

- Do not invent finished features, hardware wiring, performance numbers, model accuracy, or dataset contents that are not actually in the repo.
- Clearly separate "implemented," "planned," and "suggested" when writing docs or comments.
- When adding firmware code, keep it modular per the structure above rather than monolithic sketches.
- When adding Python code, keep it script/notebook-friendly for experimentation but organized into reusable functions/modules as the project grows.
- Prefer low-cost, realistic-for-a-student-prototype components and libraries; justify any hardware or library choice in terms of ESP32-S3 fit and project constraints.
- Never suggest testing with real/dangerous crashes to generate data. Safe data collection means normal riding plus controlled, non-dangerous events (braking, bumps, controlled falls at low speed, etc.).
- Any actuator/deployment work must start with safe stand-ins: LEDs, OLED messages, buzzers, logging, or simulated signals — not a live airbag mechanism. Real deployment hardware is a separate, later, safety-critical engineering effort (triggering reliability, accidental-deployment prevention, mechanical/gas safety, certification).
- BikeGuard is an experimental research prototype, not a certified safety device. Never present it or any component of it as reliable crash protection.

## Suggested repo structure (target, to grow into)

```
/firmware        C++/Arduino code for ESP32-S3 (modular: sensors, logging, comms, config)
/python          Data processing, visualization, feature engineering, model training
/data            Recorded datasets (raw + processed), with clear labeling scheme
/docs            Hardware wiring, calibration notes, coordinate conventions, experiment logs
/models          Trained/exported models (once they exist), including TinyML exports
```

This structure does not exist yet in full — create directories as the corresponding stage of the roadmap is actually reached, not preemptively.

## Source of truth

Prioritize information from [github.com/Balcon2/bikeguard](https://github.com/Balcon2/bikeguard) itself over general assumptions about similar projects. When the repo's actual contents (code, docs, issues) conflict with this file, the repo is correct and this file should be updated.
