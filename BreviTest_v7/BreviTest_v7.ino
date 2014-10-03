//#define DEBUGGING
#define GSM_ON

#include <EEPROM.h>

#include <Wire.h>
#include <GSM.h>

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

#ifdef GSM_ON
    // include the GSM library
    
    // PIN number if necessary
    #define PINNUMBER ""
    
    // APN information obrained from your network provider
    #define GPRS_APN       "bluevia.movistar.es" // replace with your GPRS APN
    #define GPRS_LOGIN     ""    // replace with your GPRS login
    #define GPRS_PASSWORD  "" // replace with your GPRS password
    
    // initialize the library instances
    GSMClient gsmClient;
    GPRS gprs;
    GSM gsmAccess;
    
    WebSocketClient websocketClient;
    char websocketURL[] = "54.185.178.161";
    int websocketPort = 80;
    char websocketPath[] = "/brevitest";
    char msgDeviceINIT[] = "INIT:DEVICE:0000-0000-0000-0000";
    
    #define PING_INTERVAL 30000UL
    unsigned long last_ping;
    
    #define MAX_QUEUE_SIZE 10
    String message_queue[MAX_QUEUE_SIZE];
    int message_queue_length = 0;
    extern bool gsm_setup();
    extern void gsm_loop();
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
#ifdef GSM_ON
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
 
#ifdef GSM_ON
    if (gsm_setup()) {
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

#ifdef GSM_ON
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
#ifdef GSM_ON
  gsm_loop();
  
  while (message_queue_length > 0) {
    process_message();
  }
#else
  while (true);
#endif
}


