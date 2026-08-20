# BikeGuard 🚲

An open-source project exploring a smart, ML-based crash detection and airbag system for bicycles and e-bikes.

## Overview

BikeGuard aims to detect potential crashes in real time by analyzing movement and speed data from onboard sensors.

The long-term goal is to develop a **reusable, low-cost bicycle airbag system** that can react to serious crash events.

## Core Concept

```text
Sensors
   ↓
ESP32-S3
   ↓
Motion & speed data
   ↓
Machine Learning
   ↓
Crash detection
   ↓
Airbag safety system
```

## Planned Hardware

* ESP32-S3
* Bosch BMI270 IMU
* SSD1306 OLED
* Hall-effect sensor
* Wheel magnet

## Software

* C++ / Arduino for embedded firmware
* Python for data processing and machine learning
* TinyML for future on-device inference

## Project Goals

* Detect crashes accurately
* Minimize false positives
* Process data locally in real time
* Keep the system lightweight and affordable
* Develop a reusable safety system
* Eventually create an open-source implementation

## Repository Structure

```text
Bikeguard/
├── src/          # Embedded firmware
├── ml/           # Machine-learning code
├── hardware/     # Schematics and hardware documentation
├── data/         # Sensor datasets
└── README.md
```

## Status

**Early development**

The current project is focused on sensor integration, data collection, and crash-detection research.

## Safety

BikeGuard is an experimental research project and **is not a certified safety device**. Do not rely on the current software or hardware to protect you in a real crash.

## License

Open-source project. License to be defined.
