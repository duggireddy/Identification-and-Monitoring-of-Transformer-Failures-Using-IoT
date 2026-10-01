#include "gsm_alert.h"

#include <SoftwareSerial.h>

static SoftwareSerial gsm(GSM_RX_PIN, GSM_TX_PIN);

void setupGsm() {
  gsm.begin(GSM_BAUD);
  delay(1000);
  gsm.println("AT+CMGF=1");  // SMS text mode
  delay(1000);
}

void sendAlert(const String &text) {
  String message = String(TRANSFORMER_ID) + ": " + text;
  Serial.print("Sending SMS: ");
  Serial.println(message);

  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.print("AT+CMGS=\"");
  gsm.print(PHONE_NUMBER);
  gsm.println("\"");
  delay(1000);
  gsm.print(message);
  delay(500);
  gsm.write(26);  // Ctrl+Z ends the message
  delay(3000);
}
