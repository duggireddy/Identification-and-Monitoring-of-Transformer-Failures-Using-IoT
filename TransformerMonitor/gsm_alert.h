#ifndef GSM_ALERT_H
#define GSM_ALERT_H

#include "config.h"

void setupGsm();

// Sends "<TRANSFORMER_ID>: <text>" by SMS to PHONE_NUMBER.
void sendAlert(const String &text);

#endif
