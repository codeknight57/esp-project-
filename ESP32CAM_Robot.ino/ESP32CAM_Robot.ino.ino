#include "config.h"
#include "camera_pins.h"
#include "server_handlers.h"
#include "WiFi.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "HardwareSerial.h"
#include <Arduino.h>
#include <ESP32Servo.h>  // Add servo library

// Define servo pins
#define SERVO3_PIN 12   // GPIO12 for local servo 1
#define SERVO4_PIN 13   // GPIO13 for local servo 2

// Create servo objects
Servo localServo1;
Servo localServo2;

unsigned long lastHeartbeat = 0;
const unsigned long heartbeatInterval = 30000; // 30 seconds

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

// Global variable for battery percentage
int batteryPercentage = -1;

// Global variables for speed control
int motorSpeed = 100;    // Default motor speed (0-100%)
int servoSpeed = 10;    // Default servo speed (1-20 degrees per step)

// Global variables for servo positions
int currentAngle = 90;   // Current angle for servo1 (tilt)
int currentAngle2 = 90;  // Current angle for servo2 (pan)
int currentAngle3 = 90;  // Current angle for local servo1
int currentAngle4 = 90;  // Current angle for local servo2

// Ultrasonic sensor stabilization variables
const int numReadings = 10;       // Number of readings to average
int readings[numReadings];        // Array to store readings
int readIndex = 0;                // Current index in the array
int total = 0;                    // Running total
int averageDistance = 0;          // Final averaged distance
const int maxDistance = 400;      // Maximum valid distance in cm
const int minDistance = 2;        // Minimum valid distance in cm

// Function to get a single raw ultrasonic reading
float readRawDistance() {
  // Send a 10us pulse to trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure the echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout at 30000us (30ms)
  
  // Calculate distance in cm (speed of sound = 340 m/s = 0.034 cm/us)
  return duration * 0.034 / 2;
}

// Function to initialize the distance filter
void initializeDistanceFilter() {
  // Initialize all readings to 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  
  // Fill the array with initial valid readings
  for (int i = 0; i < numReadings; i++) {
    float initialReading = readRawDistance();
    if (initialReading >= minDistance && initialReading <= maxDistance) {
      readings[i] = initialReading;
      total += initialReading;
    } else {
      readings[i] = 50; // Default to 50cm if invalid
      total += 50;
    }
  }
  averageDistance = total / numReadings;
}

// Function to read stabilized ultrasonic distance
float readDistance() {
  // Get a raw reading
  float distance = readRawDistance();
  
  // Validate the reading
  if (distance < minDistance || distance > maxDistance) {
    distance = averageDistance; // Use last valid average if reading is out of range
  }
  
  // Subtract the oldest reading from total
  total = total - readings[readIndex];
  
  // Store the new reading in the array
  readings[readIndex] = distance;
  
  // Add the new reading to total
  total = total + readings[readIndex];
  
  // Advance to the next position in the array
  readIndex = (readIndex + 1) % numReadings;
  
  // Calculate the average
  averageDistance = total / numReadings;
  
  return averageDistance;
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout detector
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize the distance filter
  initializeDistanceFilter();
  
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  
  // Initialize Serial1 for communication with second ESP32
  Serial1.begin(9600, SERIAL_8N1, 3, 1); // RX=3, TX=1 for ESP32-CAM link
  
  // Initialize local servos
  localServo1.attach(SERVO3_PIN);
  localServo2.attach(SERVO4_PIN);
  
  // Set initial positions
  localServo1.write(currentAngle3);
  localServo2.write(currentAngle4);
  
  // Send initial motor speed to second ESP32
  Serial1.print("motorSpeed:");
  Serial1.println(motorSpeed);  // motorSpeed is already initialized to 100 globally
  
  // Send initial servo speed to second ESP32
  Serial1.print("servoSpeed:");
  Serial1.println(servoSpeed);  // servoSpeed is already initialized to 10 globally
  
  // Camera configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Initialize camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed!");
    return;
  }
  
  // Start Wi-Fi AP
  WiFi.softAP(ap_ssid, ap_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Wi-Fi AP started");
  Serial.print("SSID: "); Serial.println(ap_ssid);
  Serial.print("Password: "); Serial.println(ap_password);
  Serial.print("Go to: http://"); Serial.println(IP);
  
  // Start web server
  startCameraServer();
}
void loop() {
  // Check for incoming data from second ESP32
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    data.trim();
    
    Serial.print("Received from second ESP32: ");
    Serial.println(data);
    
    // Check if it's servo position data
    if (data.startsWith("servoPositions:")) {
      String positions = data.substring(15);
      int commaIndex = positions.indexOf(',');
      if (commaIndex != -1) {
        currentAngle = positions.substring(0, commaIndex).toInt();
        currentAngle2 = positions.substring(commaIndex + 1).toInt();
      }
    }
    // Check if it's a battery value
    else if (data.startsWith("battery:")) {
      int value = data.substring(8).toInt();
      if (value >= 0 && value <= 100) {
        batteryPercentage = value;
        lastHeartbeat = millis(); // Update heartbeat on valid data
        Serial.print("Updated battery percentage to: ");
        Serial.println(batteryPercentage);
      } else {
        Serial.println("Received invalid battery value");
      }
    }
  }
  
  // Check if heartbeat is too old
  if (millis() - lastHeartbeat > heartbeatInterval) {
    Serial.println("Warning: No data from second ESP32 for 30 seconds");
    batteryPercentage = -1; // Indicate connection issue
    lastHeartbeat = millis(); // Reset timer to avoid spamming
  }
}

// Server handler implementations
esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
  
  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){ return res; }
  
  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){ res = ESP_FAIL; }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){ res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len); }
    if(res == ESP_OK){ res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY)); }
    if(fb){ esp_camera_fb_return(fb); fb = NULL; _jpg_buf = NULL; }
    else if(_jpg_buf){ free(_jpg_buf); _jpg_buf = NULL; }
    if(res != ESP_OK){ break; }
  }
  return res;
}

esp_err_t cmd_handler(httpd_req_t *req){
  char variable[128] = {0,};
  if (httpd_req_get_url_query_str(req, variable, sizeof(variable)) != ESP_OK) {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  
  // Handle motor speed control
  char motorSpeedStr[10] = {0,};
  if (httpd_query_key_value(variable, "motorSpeed", motorSpeedStr, sizeof(motorSpeedStr)) == ESP_OK) {
    int newSpeed = atoi(motorSpeedStr);
    if (newSpeed >= 0 && newSpeed <= 100) {
      motorSpeed = newSpeed;
      Serial1.print("motorSpeed:");
      Serial1.println(motorSpeed);
    }
  }
  
  // Handle servo speed control
  char servoSpeedStr[10] = {0,};
  if (httpd_query_key_value(variable, "servoSpeed", servoSpeedStr, sizeof(servoSpeedStr)) == ESP_OK) {
    int newSpeed = atoi(servoSpeedStr);
    if (newSpeed >= 1 && newSpeed <= 20) {
      servoSpeed = newSpeed;
      Serial1.print("servoSpeed:");
      Serial1.println(servoSpeed);
    }
  }
  
  // Handle commands via the "go" parameter
  char goCmd[32] = {0,};
  if (httpd_query_key_value(variable, "go", goCmd, sizeof(goCmd)) == ESP_OK) {
    // Check if it's a movement command
    if (strcmp(goCmd, "forward") == 0 || strcmp(goCmd, "backward") == 0 || 
        strcmp(goCmd, "left") == 0 || strcmp(goCmd, "right") == 0) {
      // Always forward the command to second ESP32
      Serial1.println(goCmd);
    }
    // Check if it's a servo1 command
    else if (strncmp(goCmd, "servo1:", 7) == 0) {
      // Extract the angle value
      int angle = atoi(goCmd + 7);
      // Update our tracked position
      currentAngle = angle;
      // Forward the command to second ESP32
      Serial1.println(goCmd);
      // Also control local servo3
      currentAngle3 = angle;
      localServo1.write(angle);
    }
    // Check if it's a servo2 command
    else if (strncmp(goCmd, "servo2:", 7) == 0) {
      // Extract the angle value
      int angle = atoi(goCmd + 7);
      // Update our tracked position
      currentAngle2 = angle;
      // Forward the command to second ESP32
      Serial1.println(goCmd);
      // Also control local servo4
      currentAngle4 = angle;
      localServo2.write(angle);
    }
    // Handle LED command
    else if (strcmp(goCmd, "led") == 0) {
      Serial1.println("led");
    }
  }
  
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

esp_err_t distance_handler(httpd_req_t *req){
  float distance = readDistance();
  char distance_str[10];
  snprintf(distance_str, sizeof(distance_str), "%.2f", distance);
  httpd_resp_set_type(req, "text/plain");
  return httpd_resp_send(req, distance_str, strlen(distance_str));
}

esp_err_t battery_handler(httpd_req_t *req){
  char battery_str[10];
  snprintf(battery_str, sizeof(battery_str), "%d", batteryPercentage);
  httpd_resp_set_type(req, "text/plain");
  return httpd_resp_send(req, battery_str, strlen(battery_str));
}

// Add handlers for current speed values
esp_err_t motor_speed_handler(httpd_req_t *req){
  char speed_str[10];
  snprintf(speed_str, sizeof(speed_str), "%d", motorSpeed);
  httpd_resp_set_type(req, "text/plain");
  return httpd_resp_send(req, speed_str, strlen(speed_str));
}

esp_err_t servo_speed_handler(httpd_req_t *req){
  char speed_str[10];
  snprintf(speed_str, sizeof(speed_str), "%d", servoSpeed);
  httpd_resp_set_type(req, "text/plain");
  return httpd_resp_send(req, speed_str, strlen(speed_str));
}

esp_err_t servo_position_handler(httpd_req_t *req){
  char position_str[30];  // Increased buffer size for 4 servos
  snprintf(position_str, sizeof(position_str), 
           "%d,%d,%d,%d", 
           currentAngle,    // Remote servo 1
           currentAngle2,   // Remote servo 2
           currentAngle3,   // Local servo 1
           currentAngle4);  // Local servo 2
  httpd_resp_set_type(req, "text/plain");
  return httpd_resp_send(req, position_str, strlen(position_str));
}

esp_err_t camera_settings_handler(httpd_req_t *req){
  char query[100];
  char setting[32] = {0,};
  char value[32] = {0,};
  char reset[10] = {0,};
  
  if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
    // Check if this is a reset request
    if (httpd_query_key_value(query, "reset", reset, sizeof(reset)) == ESP_OK) {
      // Reset all camera settings to default
      sensor_t * s = esp_camera_sensor_get();
      s->set_brightness(s, 0);
      s->set_contrast(s, 0);
      s->set_saturation(s, 0);
      s->set_sharpness(s, 0);
      s->set_special_effect(s, 0);
      s->set_wb_mode(s, 0);
      s->set_exposure_ctrl(s, 1);
      s->set_ae_level(s, 0);
    }
    // Process individual settings
    else if (httpd_query_key_value(query, "setting", setting, sizeof(setting)) == ESP_OK &&
        httpd_query_key_value(query, "value", value, sizeof(value)) == ESP_OK) {
      
      sensor_t * s = esp_camera_sensor_get();
      
      if (strcmp(setting, "brightness") == 0) {
        s->set_brightness(s, atoi(value));
      }
      else if (strcmp(setting, "contrast") == 0) {
        s->set_contrast(s, atoi(value));
      }
      else if (strcmp(setting, "saturation") == 0) {
        s->set_saturation(s, atoi(value));
      }
      else if (strcmp(setting, "sharpness") == 0) {
        s->set_sharpness(s, atoi(value));
      }
      else if (strcmp(setting, "special_effect") == 0) {
        s->set_special_effect(s, atoi(value));
      }
      else if (strcmp(setting, "wb_mode") == 0) {
        s->set_wb_mode(s, atoi(value));
      }
      else if (strcmp(setting, "exposure") == 0) {
        s->set_exposure_ctrl(s, 1);
        s->set_ae_level(s, atoi(value));
      }
    }
  }
  
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri="/", 
    .method=HTTP_GET, 
    .handler=index_handler, 
    .user_ctx=NULL
  };
  httpd_uri_t cmd_uri = {
    .uri="/action", 
    .method=HTTP_GET, 
    .handler=cmd_handler, 
    .user_ctx=NULL
  };
  httpd_uri_t stream_uri = {
    .uri="/stream", 
    .method=HTTP_GET, 
    .handler=stream_handler, 
    .user_ctx=NULL
  };
  httpd_uri_t distance_uri = {
    .uri="/distance", 
    .method=HTTP_GET, 
    .handler=distance_handler, 
    .user_ctx=NULL
  };
  httpd_uri_t battery_uri = {
    .uri="/battery", 
    .method=HTTP_GET, 
    .handler=battery_handler, 
    .user_ctx=NULL
  };
  // Add new endpoints for speed values
  httpd_uri_t motor_speed_uri = {
    .uri="/motorSpeed", 
    .method=HTTP_GET, 
    .handler=motor_speed_handler, 
    .user_ctx=NULL
  };
  httpd_uri_t servo_speed_uri = {
    .uri="/servoSpeed", 
    .method=HTTP_GET, 
    .handler=servo_speed_handler, 
    .user_ctx=NULL
  };
  // Add the new servo position endpoint
  httpd_uri_t servo_position_uri = {
    .uri="/servoPosition", 
    .method=HTTP_GET, 
    .handler=servo_position_handler, 
    .user_ctx=NULL
  };
  httpd_uri_t camera_settings_uri = {
    .uri="/camera", 
    .method=HTTP_GET, 
    .handler=camera_settings_handler, 
    .user_ctx=NULL
  };
  
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
    httpd_register_uri_handler(camera_httpd, &distance_uri);
    httpd_register_uri_handler(camera_httpd, &battery_uri);
    // Register the new endpoints
    httpd_register_uri_handler(camera_httpd, &motor_speed_uri);
    httpd_register_uri_handler(camera_httpd, &servo_speed_uri);
    httpd_register_uri_handler(camera_httpd, &servo_position_uri);
    httpd_register_uri_handler(camera_httpd, &camera_settings_uri);
  }
  
  config.server_port += 1; 
  config.ctrl_port += 1;
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}