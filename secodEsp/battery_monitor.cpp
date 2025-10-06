#include "battery_monitor.h"
#include <Arduino.h>
#include <HardwareSerial.h>

extern HardwareSerial MySerial;
int batteryPercentage = -1;

void setupBatteryMonitor() {
  pinMode(BATTERY_PIN, INPUT);
}

float getBatteryVoltage() {
  int rawValue = analogRead(BATTERY_PIN);
  float pinVoltage = rawValue * (3.3 / 4095.0); // ESP32 ADC resolution
  float batteryVoltage = pinVoltage * voltageDividerRatio * correctionFactor;

  // Debugging
  Serial.print("Raw ADC: ");
  Serial.print(rawValue);
  Serial.print(" | Pin Voltage: ");
  Serial.print(pinVoltage, 3);
  Serial.print(" V | Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.println(" V");

  return batteryVoltage;
}

int getBatteryPercentage() {
  float batteryVoltage = getBatteryVoltage();
  float percentage = (batteryVoltage - batteryMinVoltage) /
                     (batteryMaxVoltage - batteryMinVoltage) * 100.0;
  percentage = constrain(percentage, 0, 100);
  return (int)percentage;
}

void sendBatteryStatus() {
  float batteryVoltage = getBatteryVoltage();
  batteryPercentage = getBatteryPercentage();

  // Send compact info to MySerial
  MySerial.print("battery:");
  MySerial.println(batteryPercentage);

  // Send detailed info to Serial
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.print("V (");
  Serial.print(batteryPercentage);
  Serial.println("%)");
}
