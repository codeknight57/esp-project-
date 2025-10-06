#ifndef CONFIG_H
#define CONFIG_H

// Ultrasonic sensor pins
#define TRIG_PIN 14
#define ECHO_PIN 15

// WiFi credentials
const char* ap_ssid = "ESP32CAM2";
const char* ap_password = "12345678";

// Camera model selection (uncomment one)
#define CAMERA_MODEL_AI_THINKER
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM_B
//#define CAMERA_MODEL_WROVER_KIT

// Serial communication settings
#define SERIAL_BAUD_RATE 115200

#endif