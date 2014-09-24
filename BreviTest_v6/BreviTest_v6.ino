//#define DEBUGGING
//#define WIFI_ON

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

char serial_number[20];
unsigned long start_time;

#ifdef WIFI_ON
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
    char msgDeviceINIT[] = "INIT:DEVICE:0000-0000-0000-0000";
    
    #define PING_INTERVAL 30000UL
    unsigned long last_ping;
    
    #define MAX_QUEUE_SIZE 10
    String message_queue[MAX_QUEUE_SIZE];
    int message_queue_length = 0;
    extern bool wifi_setup();
    extern void wifi_loop();
#endif

extern void device_setup();
extern void sensor_setup();
extern void run_brevitest();
extern void get_sensor_readings();
extern void reset_x_stage();
extern void calibrate_sensors();
extern void test_sensors();
extern void move_steps();

void read_serial_number() {
  for (int i = 0; i < 19; i += 1) {
    serial_number[i] = EEPROM.read(i);
#ifdef WIFI_ON
    msgDeviceINIT[12+i] = serial_number[i];
#endif
  }
  
  Serial.print(F("Brevitest serial number: "));
  Serial.println(serial_number);
}

void setup() {
  Serial.begin(9600);
  
  delay(1000);
  
  Serial.println(F("Serial port started"));
  
  read_serial_number();
  device_setup();
  sensor_setup();
 
#ifdef WIFI_ON
    if (wifi_setup()) {
      Serial.println(F("Sending device INIT message"));
      websocketClient.sendData(msgDeviceINIT);
      start_time = millis();
      last_ping = 0;
    }
    else {
      while (true);
    }
#endif
}

#ifdef WIFI_ON
  void process_message() {
    String msg, cmd, param;
    int i, colon;
    
    msg = message_queue[0];
    message_queue_length -= 1;
    for (i = 0; i < message_queue_length; i += 1) {
      message_queue[i] = message_queue[i + 1];
    }
    
    colon = msg.indexOf(":");
    if (colon == -1) {
      cmd = msg;
      param = "";
    }
    else {
      cmd = msg.substring(0, colon);
      param = msg.substring(colon + 1);
    }
    cmd.toUpperCase();
    cmd.trim();
    Serial.print(F("Command: "));
    Serial.println(cmd);
    
    Serial.print(F("Parameter: "));
    Serial.println(param);
    
    if (cmd == "ID") {
      websocketClient.sendData(serial_number);
    }
    else if (cmd == "INIT") {
      colon = param.indexOf(":");
      cmd = param.substring(0, colon);
      if (cmd == "SERVER") {
        Serial.println(F("Server initialization received"));
      }
    }
    else if ((cmd == "PONG") || (cmd == "PING") || (cmd == "ECHO")) {
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
#endif

void loop() {
#ifdef WIFI_ON
  wifi_loop();
  
  while (message_queue_length > 0) {
    process_message();
  }
#endif

  test_sensors();
  while (true);
}


