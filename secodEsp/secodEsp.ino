#include "config.h"
#include "motor_control.h"
#include "led_control.h"
#include "battery_monitor.h"
#include <HardwareSerial.h>

// Hardware serial for external communication
HardwareSerial MySerial(2);

unsigned long lastBatteryCheck = 0;
unsigned long lastMovementTime = 0;
const unsigned long movementTimeout = 55; // Slightly above remote's 50ms interval
int motorSpeed = 50;
bool isMoving = false; // Track movement state

void handleCommand(String command) {
  command.trim();
  Serial.println("Received: " + command);
  bool movementCommandReceived = false;
  
  // Motor commands
  if (command == "forward") { 
    moveForward(motorSpeed); 
    movementCommandReceived = true;
    isMoving = true;
  }
  else if (command == "backward") { 
    moveBackward(motorSpeed); 
    movementCommandReceived = true;
    isMoving = true;
  }
  else if (command == "left") { 
    turnLeft(motorSpeed); 
    movementCommandReceived = true;
    isMoving = true;
  }
  else if (command == "right") { 
    turnRight(motorSpeed); 
    movementCommandReceived = true;
    isMoving = true;
  }
  else if (command == "stop") { 
    brakeMotors(); // Use active braking
    movementCommandReceived = true;
    isMoving = false;
  }
  
  // LED command
  else if (command == "led") {
    toggleLED();
  }
  
  // Motor speed command
  else if (command.startsWith("motorSpeed:")) {
    motorSpeed = constrain(command.substring(11).toInt(), 0, 100);
    Serial.print("Motor speed set to ");
    Serial.println(motorSpeed);
  }
  
  // Update movement time if a movement command was received
  if (movementCommandReceived) {
    lastMovementTime = millis();
  }
}

void setup() {
  
  Serial.begin(SERIAL_BAUD_RATE);
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17
  
  setupMotors();
  setupLED();
  setupBatteryMonitor();
  
  Serial.println("ESP32 Robot Initialized");
}

void loop() {
  // Read from USB Serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    handleCommand(command);
  }
  
  // Read from external UART
  if (MySerial.available()) {
    String command = MySerial.readStringUntil('\n');
    handleCommand(command);
  }
  
  // Safety stop for motors - use instant braking if no movement command received recently
  if (isMoving && (millis() - lastMovementTime > movementTimeout)) {
    brakeMotors(); // Use active braking instead of coasting
    isMoving = false;
    Serial.println("Auto-stop activated");
  }
  
 
  
   // Battery check
  if (millis() - lastBatteryCheck > batteryCheckInterval) {
    lastBatteryCheck = millis();
    sendBatteryStatus();
  }
  delay(5); // Reduced delay for faster response
}