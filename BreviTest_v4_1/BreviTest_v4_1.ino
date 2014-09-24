
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

#define channelMotor 1
#define channelSolenoid 3

#define pinFrontLimitSwitch 6
#define pinBackLimitSwitch 5

#define pinAssaySensorSDA A0
#define pinAssaySensorSCL A1
#define pinControlSensorSDA A2
#define pinControlSensorSCL A3

// NOTE: FORWARD and BACKWARD are reversed for the stepper motor!!!

#define STEP_FORWARD BACKWARD
#define STEP_BACKWARD FORWARD

#define mmPerRotation 1.0
#define solenoidDelay 2000
#define mmPerRaster 0.5
#define stepsPerRotation 200
#define rpm 200

#define number_of_wells 4   // does not include microbead and color wells
#define number_of_sensor_readings 10
#define delay_between_sensor_readings 500

#define solenoid_sustain_power 15
#define solenoid_surge_power 180
#define solenoid_surge_period 80

double rps = rpm / 60.0;
double mmPerSec = mmPerRotation * rps;
double stepsPerSec = stepsPerRotation * rps;
double delayAtFullSpeed = 1000.0 / (stepsPerRotation * rps);
double mmPerStep = mmPerRotation / stepsPerRotation;
int stepsPerRaster = int(round(mmPerRaster / mmPerStep));
double secsPerRaster = stepsPerRaster / stepsPerSec;
int solenoidDelayAfterRaster = int(round(solenoidDelay - secsPerRaster * 1000.0));

Adafruit_MotorShield AFMS;
Adafruit_StepperMotor *motor;
Adafruit_DCMotor *solenoid;

#define HELLO_INTERVAL 3000UL

//char ssid2[] = "AlphaDev Wifi 2";     //  your network SSID (name) 
//char pass2[] = "alpha123";    // your network password
//char ssid[] = "Linbeck Home";     //  your network SSID (name) 
//char pass[] = "2january88";    // your network password
//int wifi_status = WL_IDLE_STATUS;     // the Wifi radio's status
//WiFiClient client;
//SocketIOClient websocketClient;
//IPAddress ip_address;

//extern void wifi_setup();
//extern void wifi_loop();
extern void device_setup();
extern void sensor_setup();
extern void run_brevitest();
extern void read_sensors();
extern void get_sensor_readings();
extern void reset_x_stage();
extern void calibrate_sensors();
extern void move_mm(float mm);
extern void test_sensors();

struct RawSensorReading {
  uint16_t r;
  uint16_t g;
  uint16_t b;
  uint16_t c;
};

struct SensorReading {
  RawSensorReading raw;
  struct norm {
    float r;
    float g;
    float b;
  } norm;
  int colorTemp;
  int lux;
  int count;
  
  SensorReading() {
    raw.r = 0;
    raw.g = 0;
    raw.b = 0;
    raw.c = 0;
    norm.r = 0;
    norm.g = 0;
    norm.b = 0;
    colorTemp = 0;
    lux = 0;
    count = 0;
  }
};

struct SensorReadingDiff {
  struct raw {
    int r;
    int g;
    int b;
    int c;
  } raw;
  
  struct norm {
    float r;
    float g;
    float b;
  } norm;
  
  SensorReadingDiff() {
    raw.r = 0;
    raw.g = 0;
    raw.b = 0;
    raw.c = 0;
    
    norm.r = 0;
    norm.g = 0;
    norm.b = 0;
  }
};

struct SensorSample {
  long r;
  long g;
  long b;
  long c;
  int count;
  
  SensorSample() {
    r = 0;
    g = 0;
    b = 0;
    c = 0;
    count = 0;
  }
};

void setup() {
  Serial.begin(9600);
  Serial.println(F("Serial port started"));
  delay(500);
  
  // setup sensors
  sensor_setup();
//  calibrate_sensors();

  // set up stepping motor and solenoid
  device_setup();

  // set up wifi
  //wifi_setup();
}

void loop() {
//  wifi_loop();
  run_brevitest();

//  delay(10000);
  
//    test_sensors();
    while (true);
}


