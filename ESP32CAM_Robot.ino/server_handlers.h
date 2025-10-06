#ifndef SERVER_HANDLERS_H
#define SERVER_HANDLERS_H
#include "esp_http_server.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"
#include "config.h"
#include "html_content.h"
#define PART_BOUNDARY "123456789000000000000987654321"
extern httpd_handle_t camera_httpd;
extern httpd_handle_t stream_httpd;
extern int batteryPercentage;
extern int motorSpeed;    // Motor speed (0-100%)
extern int servoSpeed;    // Servo speed (1-20 degrees per step)
extern int currentAngle;   // Current angle for servo1 (tilt)
extern int currentAngle2;  // Current angle for servo2 (pan)

// Function prototypes
esp_err_t index_handler(httpd_req_t *req);
esp_err_t stream_handler(httpd_req_t *req);
esp_err_t cmd_handler(httpd_req_t *req);
esp_err_t distance_handler(httpd_req_t *req);
esp_err_t battery_handler(httpd_req_t *req);
esp_err_t motor_speed_handler(httpd_req_t *req);
esp_err_t servo_speed_handler(httpd_req_t *req);
esp_err_t servo_position_handler(httpd_req_t *req);
esp_err_t camera_settings_handler(httpd_req_t *req);
void startCameraServer();
#endif