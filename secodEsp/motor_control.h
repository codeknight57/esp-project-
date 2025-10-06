#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "config.h"

void setupMotors();
void stopMotors();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void brakeMotors();

#endif
