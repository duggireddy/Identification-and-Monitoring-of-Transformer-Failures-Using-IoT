#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"

// Averaged current readings, indexed by CurrentChannel.
extern float ctAverage[CT_COUNT];

void setupSensors();

// Takes one sample from every current sensor; returns true once SAMPLES
// samples are collected and ctAverage has been updated.
bool sampleCurrents();

float readTemperature();  // degrees C
float readGasVoltage();   // volts
int   readOilDistance();  // cm to the oil surface, -1 if no echo

void printReadings(float temperature, float gasVoltage, int oilDistance);

#endif
