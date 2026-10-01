#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ---------------- alert settings ----------------
#define PHONE_NUMBER   "+91XXXXXXXXXX"  // line man's mobile number
#define TRANSFORMER_ID "T/F 1234"

// ---------------- thresholds ----------------
const float PHASE_PRESENT_THRESHOLD = 5;     // averaged ADC counts, above = phase healthy
const float TEMP_LIMIT_C            = 70.0;  // trip above this oil temperature
const float GAS_LIMIT_V             = 2.5;   // trip above this MQ-9 output voltage (max 5 V)
const int   OIL_LOW_DISTANCE_CM     = 10;    // sensor-to-oil distance above this = low oil (calibrate to your tank)

// ---------------- sampling ----------------
const int SAMPLES   = 30;  // current samples averaged per evaluation
const int SAMPLE_MS = 30;  // delay between samples

// ---------------- pins ----------------
const int CT_COUNT = 7;
enum CurrentChannel { HT_R, HT_Y, HT_B, LT_R, LT_Y, LT_B, HT_TEST };

const int TEMP_SENSOR_PIN = A15;
const int GAS_SENSOR_PIN  = A7;
const int RELAY_PIN       = 13;
const int TRIG_PIN        = 14;
const int ECHO_PIN        = 15;
const int GSM_RX_PIN      = 4;  // connect to SIM900 TX
const int GSM_TX_PIN      = 3;  // connect to SIM900 RX

const long SERIAL_BAUD = 19200;
const long GSM_BAUD    = 9600;

#endif
