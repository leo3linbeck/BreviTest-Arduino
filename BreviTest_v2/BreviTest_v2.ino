#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include "Adafruit_TCS34725.h"
#include "Adafruit_MotorShield.h"
#include "utility/Adafruit_PWMServoDriver.h"

#define channelMotor 1
#define channelSolenoid 3

#define pinFrontLimitSwitch 6
#define pinBackLimitSwitch 5

#define pinAssaySensorVin A0
#define pinControlSensorVin A1
#define pinAssaySensorLED A2
#define pinControlSensorLED A3

// NOTE: FORWARD and BACKWARD are reversed for the stepper motor!!!

#define STEP_FORWARD BACKWARD
#define STEP_BACKWARD FORWARD

#define mmPerRotation 1.0
#define solenoidDelay 2000
#define mmPerRaster 0.5
#define stepsPerRotation 200
#define rpm 200

#define number_of_wells 5   // does not include analyte, antibody, and color wells
#define number_of_sensor_readings 10
#define delay_between_sensor_readings 500

#define solenoid_power 180

double rps = rpm / 60.0;
double mmPerSec = mmPerRotation * rps;
double stepsPerSec = stepsPerRotation * rps;
double delayAtFullSpeed = 1000.0 / (stepsPerRotation * rps);
double mmPerStep = mmPerRotation / stepsPerRotation;
int stepsPerRaster = int(round(mmPerRaster / mmPerStep));
double secsPerRaster = stepsPerRaster / stepsPerSec;
int solenoidDelayAfterRaster = int(round(solenoidDelay - secsPerRaster * 1000.0));

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *motor = AFMS.getStepper(stepsPerRotation, channelMotor);
Adafruit_DCMotor *solenoid = AFMS.getMotor(channelSolenoid);

char ssid2[] = "AlphaDev Wifi 2";     //  your network SSID (name) 
char pass2[] = "alpha123";    // your network password
char ssid[] = "Linbeck Home";     //  your network SSID (name) 
char pass[] = "2january88";    // your network password
int wifi_status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiServer server(80);
String httpRequest;
IPAddress ip_address;

extern void wifi_setup();
extern void wifi_loop();
extern void device_setup();
extern void sensor_setup();
extern void run_brevitest();
extern void read_sensors();
extern void get_sensor_readings();
extern void reset_x_stage();

void setup() {
  Serial.begin(9600);
  Serial.println(F("Serial port started"));
  delay(500);
  
  // setup sensors
//  sensor_setup();

  // set up stepping motor and solenoid
  device_setup();
  
  // set up wifi
//  wifi_setup();
}

void loop() {
//  wifi_loop();
  run_brevitest();
}


