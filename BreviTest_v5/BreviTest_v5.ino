//#define DEBUGGING

#include <EEPROM.h>

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <Base64.h>
#include <global.h>
#include <sha1.h>
#include <WebSocketClient.h>

#include "Adafruit_TCS34725.h"
#include "Adafruit_MotorShield.h"
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS;
Adafruit_StepperMotor *motor;
Adafruit_DCMotor *solenoid;

Adafruit_TCS34725 assaySensor;
Adafruit_TCS34725 controlSensor;

WiFiClient wifiClient;
//char ssid[] = "AlphaDev Wifi 2";     //  Fannin network ID
//char pass[] = "alpha123";    // Fannin network password
char ssid[] = "Linbeck Home";     //  L3 network ID
char pass[] = "2january88";    // L3 network password
int wifi_status = WL_IDLE_STATUS;     // the Wifi radio's status

WebSocketClient websocketClient;
char websocketURL[] = "172.16.121.98";
int websocketPort = 8081;
char websocketPath[] = "/brevitest";
//char websocketURL[] = "echo.websocket.org";
//int websocketPort = 80;
//char websocketPath[] = "/";

#define PING_INTERVAL 30000UL
unsigned long start_time;
unsigned long last_ping;

#define MAX_QUEUE_SIZE 10
String message_queue[MAX_QUEUE_SIZE];
int message_queue_length = 0;
char serial_number[20];

extern void wifi_setup();
extern void wifi_loop();
extern void device_setup();
extern void sensor_setup();
extern void run_brevitest();
extern void read_sensors();
extern void get_sensor_readings();
extern void reset_x_stage();
extern void calibrate_sensors();
extern void test_sensors();
extern void move_steps();

void read_serial_number() {
  for (int i = 0; i < 19; i += 1) {
    serial_number[i] = EEPROM.read(i);
  }
  
  Serial.print(F("Brevitest serial number: "));
  Serial.println(serial_number);
}

void setup() {
  Serial.begin(9600);
  
  delay(1000);
  
  Serial.println(F("Serial port started"));
  
  read_serial_number();
  
  wifi_setup();
  sensor_setup();
  device_setup();

  start_time = millis();
  last_ping = 0;
}

void process_message() {
  String msg, cmd, param;
  unsigned int i, colon;
  
  msg = message_queue[0];
  message_queue_length -= 1;
  for (i = 0; i < message_queue_length; i += 1) {
    message_queue[i] = message_queue[i + 1];
  }
  
  colon = msg.indexOf(":");
  cmd = msg.substring(0, colon);
  cmd.toUpperCase();
  cmd.trim();
  Serial.print(F("Command: "));
  Serial.println(cmd);
  
  param = msg.substring(colon + 1);
  Serial.print(F("Parameter: "));
  Serial.println(param);
  
  if (cmd == "ID") {
    websocketClient.sendData(serial_number);
  }
  else if ((cmd == "INIT") || (cmd == "PONG") || (cmd == "PING") || (cmd == "ECHO")) {
    Serial.print(F("Received: "));
    Serial.println(msg);
  }
  else if (cmd == "RUN") {
    run_brevitest();
  }
  else if (cmd == "RESET") {
    reset_x_stage();
  }
  else if (cmd == "TEST_SENSORS") {
    test_sensors();
  }
  else if (cmd == "READ_SENSORS") {
    get_sensor_readings();
  }
  else {
    Serial.println(F("Command not implemented."));
  }
}

void loop() {
  wifi_loop();
  
  while (message_queue_length > 0) {
    process_message();
  }
}


