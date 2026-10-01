# Identification and Monitoring of Transformer Failures Using IoT

The transformer is one of the most important pieces of equipment in an electrical network. Tracking its condition and maintaining it properly keeps power supply continuous and reduces economic losses. This project detects common distribution-transformer faults and responds to them automatically: it trips the transformer when needed and sends an SMS to the line man naming the exact fault.

Bachelor thesis project, Dept. of EEE, SIETK. The full report is in [docs/Bachelor_thesis_report.pdf](docs/Bachelor_thesis_report.pdf).

## Features

| Fault / condition | Detection | Response |
|---|---|---|
| HT fuse blown (R, Y, B, any two, or all three phases) | Current sensors on the HT side, cross-checked with a test line so a grid outage is not reported as a fuse fault | SMS naming the phase(s) |
| LT fuse blown (R, Y, B, any two, or all three phases) | Current sensors on the LT side (only evaluated while HT is healthy) | SMS naming the phase(s) |
| Abnormal heating | LM35 on the conservator tank, above 70 °C | Transformer tripped through the 3-phase SSR + SMS |
| Gas leakage | MQ-9 gas sensor on the conservator tank | Transformer tripped + SMS |
| Low oil level | HC-SR04 ultrasonic sensor measuring the distance to the oil surface | SMS |
| Live monitoring | All readings printed on the serial monitor every ~1 s | — |

An SMS is sent once when a fault first appears, not repeatedly while it persists. After a trip, the relay stays open until the Arduino is reset.

Example SMS: `T/F 1234: LT fuse fault in B phase`

## Project structure

```
TransformerMonitor/
├── TransformerMonitor.ino   setup() and the main loop
├── config.h                 phone number, thresholds, pin assignments
├── sensors.h / .cpp         current, temperature, gas and oil-level readings
├── fault_detection.h / .cpp HT/LT fuse, heating, gas and oil checks; relay trip
└── gsm_alert.h / .cpp       SIM900 SMS alerts
docs/
└── Bachelor_thesis_report.pdf
```

## How it works

1. Each current sensor is sampled 30 times, 30 ms apart, and averaged.
2. An average at or below `PHASE_PRESENT_THRESHOLD` means that phase is missing.
3. Temperature, gas and oil level are read once per cycle and compared against their limits.
4. Faults are sent as SMS through the SIM900 GSM module using AT commands.

## Hardware

- Arduino Mega 2560
- GSM module SIM900 + SIM card
- 7 × 5 A AC current sensor modules (HT R/Y/B, LT R/Y/B, HT test line)
- Gas sensor MQ-9
- Temperature sensor LM35
- Ultrasonic sensor HC-SR04
- 3-phase solid state relay
- 3-phase transformer (2 kVA step-down used in the demo)
- Resistive load (12 A, standing in for a motor)
- Banana plug probes, jumper wires
- Mobile phone to receive notifications

## Wiring

| Signal | Mega 2560 pin |
|---|---|
| HT R / Y / B current sensors | A0 / A1 / A2 |
| LT R / Y / B current sensors | A3 / A4 / A5 |
| HT test line current sensor | A6 |
| MQ-9 analog output (AO) | A7 |
| LM35 output | A15 |
| Solid state relay control | D13 |
| HC-SR04 Trig / Echo | D14 / D15 |
| SIM900 TX / RX | D4 / D3 |

The gas, temperature and ultrasonic sensors are mounted on the conservator tank. The ultrasonic sensor points down at the oil surface.

## Setup

1. Install the latest [Arduino IDE](https://www.arduino.cc/en/software).
2. Open `TransformerMonitor/TransformerMonitor.ino` (the IDE opens all files in the folder as tabs), select **Tools → Board → Arduino Mega or Mega 2560** and the correct port.
3. Edit `config.h`:
   - `PHONE_NUMBER`: the line man's mobile number, with country code
   - `TRANSFORMER_ID`: the label included in every SMS
   - `TEMP_LIMIT_C`, `GAS_LIMIT_V`, `OIL_LOW_DISTANCE_CM`, `PHASE_PRESENT_THRESHOLD`: calibrate these to your tank and sensors
4. Upload, then open the Serial Monitor at **19200 baud** to watch the live readings.

The SIM900 is driven at 9600 baud. If your module uses a different rate, change `GSM_BAUD` in `config.h`.

## Budget (thesis build)

| Item | Price (₹) |
|---|---|
| Arduino | 900 |
| 3-phase solid state relay | 4,000 |
| GSM module | 1,530 |
| Gas sensor | 290 |
| Ultrasonic sensor | 150 |
| Temperature sensor | 335 |
| Transformer | 51,000 |
| Resistive load | 22,000 |
| Furniture design | 3,800 |
| Connecting wires | 1,000 |
| **Total** | **85,005** |

## Team

- D. Harinath Reddy — proposal & research
- Faysal Ahammed — logic programming
- C. Bhanu Prakash Reddy — technical hardware
- J. Lakshmi Theja — technical design & documentation
- A. G. Gagan Chandra — helper
