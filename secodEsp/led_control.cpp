#include <Arduino.h>     // <-- Add this line
#include "led_control.h"

bool ledState = false;

void setupLED() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  Serial.print("LED state: ");
  Serial.println(ledState ? "ON" : "OFF");
}
