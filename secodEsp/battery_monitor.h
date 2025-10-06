#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "config.h"

void setupBatteryMonitor();
float getBatteryVoltage();
int getBatteryPercentage();
void sendBatteryStatus();

#endif
