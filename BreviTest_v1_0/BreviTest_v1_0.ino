#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
#include <LiquidCrystal.h>

#define channelMotor 1
#define channelSolenoid 3

#define pinFrontLimitSwitch 11
#define pinBackLimitSwitch 12

#define pinButtonPowerLight 3
#define pinButtonRun 13
#define pinButtonRunLight 4
#define pinAssaySensor A0
#define pinControlSensor A1

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

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

#define solenoid_power 180

double rps = rpm / 60.0;
double mmPerSec = mmPerRotation * rps;
double stepsPerSec = stepsPerRotation * rps;
double delayAtFullSpeed = 1000.0 / (stepsPerRotation * rps);
double mmPerStep = mmPerRotation / stepsPerRotation;
int stepsPerRaster = int(round(mmPerRaster / mmPerStep));
double secsPerRaster = stepsPerRaster / stepsPerSec;
int solenoidDelayAfterRaster = int(round(solenoidDelay - secsPerRaster * 1000.0));

int stateButtonRun;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *motor = AFMS.getStepper(stepsPerRotation, channelMotor);
Adafruit_DCMotor *solenoid = AFMS.getMotor(channelSolenoid);

void setup() {
    // initialize pins
  pinMode(pinFrontLimitSwitch, INPUT);
  pinMode(pinBackLimitSwitch, INPUT);
  pinMode(pinAssaySensor, INPUT);
  pinMode(pinControlSensor, INPUT);
  pinMode(pinButtonRun, INPUT_PULLUP);
  
  pinMode(pinButtonRunLight, OUTPUT);  
  pinMode(pinButtonPowerLight, OUTPUT);
  
  digitalWrite(pinButtonRunLight, LOW);
  digitalWrite(pinButtonPowerLight, HIGH);
  
  Serial.begin(9600);
  
    // set up stepping motor
  Serial.println("Initializing shield, motor and solenoid");
  AFMS.begin();
  motor->setSpeed(rpm);
  solenoid->setSpeed(solenoid_power);
  
  reset_x_stage();
}

void reset_x_stage() {
  Serial.println("Resetting X stage");
  move_steps(30000, STEP_BACKWARD, DOUBLE);
  motor->release();
  delay(1000);
}

void loop() {
  int rVal, rVal2;
  
  rVal = digitalRead(pinButtonRun);
  delay(DEBOUNCE);
  rVal2 = digitalRead(pinButtonRun);
  if (rVal == rVal2) {
    Serial.print("rval = ");
    Serial.print(rVal);
    Serial.print(", stateButtonRun = ");
    Serial.println(stateButtonRun);
    if (rVal != stateButtonRun) {
      if (rVal == LOW) {
        Serial.println("Running test");
        brevitest_run();
      }
      stateButtonRun = HIGH;
    }
  }
}

void brevitest_run() {
  bt_setup();

  bt_move_to_second_well();
  bt_raster_well();
  bt_move_back_to_analyte_well();
  bt_raster_well();
  bt_move_to_next_well();
  bt_skip_well();
  
  for (int i = 0; i < number_of_wells; i += 1) {
    bt_raster_well();
    bt_move_to_next_well();
  }
  
  bt_raster_last_well();
  bt_position_sensors();
  bt_read_sensors();

  bt_cleanup();
}

void bt_setup() {
  digitalWrite(pinButtonRunLight, HIGH);
  reset_x_stage();
}

void bt_move_to_second_well() {
  move_mm(15.0);
}

void bt_move_back_to_analyte_well() {
  move_mm(-16.0);
}

void solenoid_out() {
    Serial.println("Shifting out...");
    solenoid->run(RELEASE);
}

void solenoid_in() {
    Serial.println("Shifting in...");
    solenoid->run(FORWARD);
}

void raster_well(double mm) {
  int number_of_rasters = int(round(mm / mmPerRaster));
  
  for (int i = 0; i < number_of_rasters; i += 1) {
    move_steps(stepsPerRaster, STEP_FORWARD, SINGLE);
    delay(solenoidDelayAfterRaster);
    solenoid_in();
    delay(solenoidDelay);
    solenoid_out();
  }
}

void move_steps(int steps, int dir, int rate) {
  Serial.print("Moving ");
  Serial.print(steps);
  Serial.println(" steps");
  for (int i = 0; i < steps; i += 1) {
    if (dir == STEP_FORWARD && digitalRead(pinFrontLimitSwitch) == HIGH) {
      Serial.println("Front limit switch tripped");
      return;
    }
    if (dir == STEP_BACKWARD && digitalRead(pinBackLimitSwitch) == HIGH) {
      Serial.println("Back limit switch tripped");
      return;
    }
    motor->step(1, dir, rate);
  }
}

void move_mm(double mm) {
  int steps = int(round(abs(mm) / mmPerStep));
  Serial.print("Steps: ");
  Serial.println(steps);
  if (mm > 0) {
    move_steps(steps, STEP_FORWARD, SINGLE);
  }
  else {
    move_steps(steps, STEP_BACKWARD, SINGLE);
  }
}

void bt_raster_first_well() {
  raster_well(6.0);
}

void bt_move_to_next_well() {
  move_mm(4.0);
}

void bt_skip_well() {
  move_mm(10.0);
}

void bt_raster_well() {
  raster_well(6.0);
}

void bt_raster_last_well() {
  raster_well(8.0);
}

void bt_position_sensors() {
  move_mm(-16.0);
}

void bt_read_sensors() {
  double assay = 0.0;
  double control = 0.0;
  
  for (int i = 0; i < number_of_sensor_readings; i += 1) {
    assay += analogRead(pinAssaySensor);
    control += analogRead(pinControlSensor);
    delay(delay_between_sensor_readings);
  }
  
  Serial.print("Assay reading: ");
  Serial.println(assay / number_of_sensor_readings);
  Serial.print("Control reading: ");
  Serial.println(control / number_of_sensor_readings);
}

void bt_cleanup() {
  solenoid_out();
  motor->release();
  digitalWrite(pinButtonRunLight, LOW);
}
