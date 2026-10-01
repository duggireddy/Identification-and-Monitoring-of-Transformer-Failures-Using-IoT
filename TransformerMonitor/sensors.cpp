#include "sensors.h"

static const int ctPins[CT_COUNT] = {A0, A1, A2, A3, A4, A5, A6};

static int ctSamples[CT_COUNT][SAMPLES];
static int sampleIndex = 0;

float ctAverage[CT_COUNT];

void setupSensors() {
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

bool sampleCurrents() {
  for (int i = 0; i < CT_COUNT; i++) {
    ctSamples[i][sampleIndex] = analogRead(ctPins[i]);
  }
  sampleIndex++;

  if (sampleIndex < SAMPLES) {
    return false;
  }
  sampleIndex = 0;

  for (int i = 0; i < CT_COUNT; i++) {
    long sum = 0;
    for (int s = 0; s < SAMPLES; s++) {
      sum += ctSamples[i][s];
    }
    ctAverage[i] = (float)sum / SAMPLES;
  }
  return true;
}

// LM35: 10 mV per degree C, 5 V ADC reference
float readTemperature() {
  return analogRead(TEMP_SENSOR_PIN) * 500.0 / 1023.0;
}

float readGasVoltage() {
  return analogRead(GAS_SENSOR_PIN) * 5.0 / 1023.0;
}

// HC-SR04 mounted above the oil, pointing down at the surface.
int readOilDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) {
    return -1;
  }
  return duration * 0.034 / 2;
}

void printReadings(float temperature, float gasVoltage, int oilDistance) {
  Serial.print("Temperature (C): ");    Serial.println(temperature);
  Serial.print("Oil distance (cm): ");  Serial.println(oilDistance);
  Serial.print("Gas sensor (V): ");     Serial.println(gasVoltage);
  Serial.print("HT R: ");    Serial.println(ctAverage[HT_R]);
  Serial.print("HT Y: ");    Serial.println(ctAverage[HT_Y]);
  Serial.print("HT B: ");    Serial.println(ctAverage[HT_B]);
  Serial.print("LT R: ");    Serial.println(ctAverage[LT_R]);
  Serial.print("LT Y: ");    Serial.println(ctAverage[LT_Y]);
  Serial.print("LT B: ");    Serial.println(ctAverage[LT_B]);
  Serial.print("HT test: "); Serial.println(ctAverage[HT_TEST]);
}
