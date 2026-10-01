#include "fault_detection.h"

#include "gsm_alert.h"
#include "sensors.h"

static const byte ALL_PHASES = 0b111;

// Fault state from the last evaluation.
static byte htFaultReported = 0;  // bitmask of missing HT phases (bit0=R, bit1=Y, bit2=B)
static byte ltFaultReported = 0;  // bitmask of missing LT phases
static bool tempFaultReported = false;
static bool gasFaultReported  = false;
static bool oilFaultReported  = false;
static bool tripped = false;       // relay stays open after a trip until the board is reset

void setupProtection() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // transformer connected
}

static void tripTransformer() {
  if (!tripped) {
    tripped = true;
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay opened, transformer disconnected");
  }
}

static bool phasePresent(int channel) {
  return ctAverage[channel] > PHASE_PRESENT_THRESHOLD;
}

// Bitmask of the missing phases among R, Y, B starting at the given channel.
static byte missingPhases(int firstChannel) {
  byte mask = 0;
  for (int p = 0; p < 3; p++) {
    if (!phasePresent(firstChannel + p)) {
      mask |= 1 << p;
    }
  }
  return mask;
}

static String phaseNames(byte mask) {
  const char *names[] = {"R", "Y", "B"};
  String text = "";
  for (int p = 0; p < 3; p++) {
    if (mask & (1 << p)) {
      if (text.length() > 0) text += " and ";
      text += names[p];
    }
  }
  return text;
}

static void reportFuseFault(const char *side, byte missing) {
  if (missing == ALL_PHASES) {
    sendAlert(String(side) + " 3-phase fuse fault");
  } else {
    sendAlert(String(side) + " fuse fault in " + phaseNames(missing) + " phase");
  }
}

void checkHtFuses() {
  byte missing = missingPhases(HT_R);

  // All three HT phases dead with no supply on the test line is a grid outage, not a fuse fault.
  if (missing == ALL_PHASES && !phasePresent(HT_TEST)) {
    Serial.println("HT supply not available");
    missing = 0;
  }

  if (missing != 0 && missing != htFaultReported) {
    reportFuseFault("HT", missing);
  }
  htFaultReported = missing;
}

// LT faults are only meaningful while all HT phases are healthy.
void checkLtFuses() {
  byte missing = 0;
  if (missingPhases(HT_R) == 0) {
    missing = missingPhases(LT_R);
  }

  if (missing != 0 && missing != ltFaultReported) {
    reportFuseFault("LT", missing);
  }
  ltFaultReported = missing;
}

void checkTemperature(float temperature) {
  bool fault = temperature > TEMP_LIMIT_C;
  if (fault && !tempFaultReported) {
    tripTransformer();
    sendAlert("Abnormal heating " + String(temperature, 1) + " C, transformer shut down");
  }
  tempFaultReported = fault;
}

void checkGas(float gasVoltage) {
  bool fault = gasVoltage > GAS_LIMIT_V;
  if (fault && !gasFaultReported) {
    tripTransformer();
    sendAlert("Gas leakage detected, transformer shut down");
  }
  gasFaultReported = fault;
}

void checkOilLevel(int oilDistance) {
  bool fault = oilDistance > OIL_LOW_DISTANCE_CM;
  if (fault && !oilFaultReported) {
    sendAlert("Oil level low in conservator");
  }
  oilFaultReported = fault;
}
