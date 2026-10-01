#ifndef FAULT_DETECTION_H
#define FAULT_DETECTION_H

#include "config.h"

void setupProtection();

// Each check sends an SMS only when its fault first appears.
void checkHtFuses();
void checkLtFuses();
void checkTemperature(float temperature);
void checkGas(float gasVoltage);
void checkOilLevel(int oilDistance);

#endif
