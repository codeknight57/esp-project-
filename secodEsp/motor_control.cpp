#include "motor_control.h"
#include <Arduino.h>

void setupMotors() {
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);

  ledcAttach(ENABLE_1_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(ENABLE_2_PIN, PWM_FREQ, PWM_RESOLUTION);

  stopMotors();
}

void stopMotors() {
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, LOW);

  ledcWrite(ENABLE_1_PIN, 0);
  ledcWrite(ENABLE_2_PIN, 0);
}

void moveForward(int speed) {
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, HIGH);
  digitalWrite(MOTOR_2_PIN_2, LOW);

  int duty = map(speed, 0, 100, 0, 255);
  ledcWrite(ENABLE_1_PIN, duty);
  ledcWrite(ENABLE_2_PIN, duty);
}

void moveBackward(int speed) {
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, HIGH);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, HIGH);

  int duty = map(speed, 0, 100, 0, 255);
  ledcWrite(ENABLE_1_PIN, duty);
  ledcWrite(ENABLE_2_PIN, duty);
}

void turnLeft(int speed) {
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, HIGH);
  digitalWrite(MOTOR_2_PIN_1, HIGH);
  digitalWrite(MOTOR_2_PIN_2, LOW);

  int duty = map(speed, 0, 100, 0, 255);
  ledcWrite(ENABLE_1_PIN, duty);
  ledcWrite(ENABLE_2_PIN, duty);
}

void turnRight(int speed) {
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  digitalWrite(MOTOR_2_PIN_1, LOW);
  digitalWrite(MOTOR_2_PIN_2, HIGH);

  int duty = map(speed, 0, 100, 0, 255);
  ledcWrite(ENABLE_1_PIN, duty);
  ledcWrite(ENABLE_2_PIN, duty);
}

void brakeMotors() {
  // Set both control pins to HIGH for each motor to enable active braking
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, HIGH);
  digitalWrite(MOTOR_2_PIN_1, HIGH);
  digitalWrite(MOTOR_2_PIN_2, HIGH);
  
  // Keep PWM at maximum for strongest braking effect
  ledcWrite(ENABLE_1_PIN, 255);
  ledcWrite(ENABLE_2_PIN, 255);
}