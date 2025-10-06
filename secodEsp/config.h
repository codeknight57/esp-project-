#ifndef CONFIG_H
#define CONFIG_H

// Motor pins
#define MOTOR_1_PIN_1    27
#define MOTOR_1_PIN_2    26
#define ENABLE_1_PIN     14

#define MOTOR_2_PIN_1    25
#define MOTOR_2_PIN_2    33
#define ENABLE_2_PIN     32

// Servo pins
#define SERVO_PAN_PIN    4
#define SERVO_TILT_PIN   18

// LED pin
#define LED_PIN          23

// Battery monitoring pin
#define BATTERY_PIN      34

// Serial communication
#define SERIAL_BAUD_RATE 115200

// Battery monitoring settings for 3S Lithium Battery (≈12V pack)
const long batteryCheckInterval = 10000; // 10s
const float batteryMinVoltage = 9.0;     // 0%
const float batteryMaxVoltage = 12.6;    // 100%
extern int batteryPercentage;

// Voltage divider ratio (from your resistors)
const float voltageDividerRatio = 5.55;

// Calibration factor (adjust to match multimeter)
const float correctionFactor = 0.94;

// Servo settings
const int servoMinAngle = 0;
const int servoMaxAngle = 180;
const int servoPanInitial = 90;
const int servoTiltInitial = 90;

// Motor PWM settings
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

#endif
