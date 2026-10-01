/*
 * Identification and Monitoring of Transformer Failures Using IoT
 * Board: Arduino Mega 2560
 *
 * Wiring
 *   HT R phase current sensor  -> A0
 *   HT Y phase current sensor  -> A1
 *   HT B phase current sensor  -> A2
 *   LT R phase current sensor  -> A3
 *   LT Y phase current sensor  -> A4
 *   LT B phase current sensor  -> A5
 *   HT test (supply) sensor    -> A6
 *   MQ-9 gas sensor (AO)       -> A7
 *   LM35 temperature sensor    -> A15
 *   3-phase SSR control        -> D13
 *   HC-SR04 trigger            -> D14
 *   HC-SR04 echo               -> D15
 *   SIM900 GSM TX -> D4, RX -> D3
 *
 * Settings and thresholds live in config.h.
 */

#include "config.h"
#include "fault_detection.h"
#include "gsm_alert.h"
#include "sensors.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  setupProtection();
  setupSensors();
  setupGsm();
  Serial.println("Transformer monitoring started");
}

void loop() {
  bool cycleComplete = sampleCurrents();
  delay(SAMPLE_MS);
  if (!cycleComplete) {
    return;
  }

  float temperature = readTemperature();
  float gasVoltage  = readGasVoltage();
  int   oilDistance = readOilDistance();

  printReadings(temperature, gasVoltage, oilDistance);

  checkHtFuses();
  checkLtFuses();
  checkTemperature(temperature);
  checkGas(gasVoltage);
  checkOilLevel(oilDistance);
}
