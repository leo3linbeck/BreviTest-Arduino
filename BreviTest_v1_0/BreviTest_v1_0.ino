#include <AFMotor.h>

#define channelMotor 1
#define channelSolenoid 2

#define pinFrontLimitSwitch 5
#define pinBackLimitSwitch 7

#define pinButtonRun 2
#define pinButtonLight 4
#define pinAssaySensor A0
#define pinControlSensor A1

#define mmPerRotation 1.0
#define solenoidDelay 500
#define mmPerRaster 0.5
#define stepsPerRotation 200.0
#define rpm 100.0

#define number_of_wells 6   // does not include analyte and color wells
#define number_of_sensor_readings 10
#define delay_between_sensor_readings 500

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

double rps = rpm / 60.0;
double mmPerSec = mmPerRotation * rps;
double stepsPerSec = stepsPerRotation * rps;
double delayAtFullSpeed = 1000.0 / (stepsPerRotation * rps);
double mmPerStep = mmPerRotation / stepsPerRotation;
int stepsPerRaster = int(round(mmPerRaster / mmPerStep));
double secsPerRaster = stepsPerRaster / stepsPerSec;
int solenoidDelayAfterRaster = int(round(solenoidDelay - secsPerRaster * 1000.0));

int stateButtonRun;

AF_Stepper motor(stepsPerRotation, channelMotor);
AF_DCMotor solenoid(channelSolenoid, MOTOR12_8KHZ);

void setup() {
  Serial.begin(9600);
  
    // set up stepping motor
  Serial.println("Initializing motor and solenoid");
  motor.setSpeed(rpm);
  solenoid.setSpeed(255);
  
    // initialize pins
  Serial.println("Initializing pins");
  pinMode(pinFrontLimitSwitch, INPUT);
  pinMode(pinBackLimitSwitch, INPUT);
  pinMode(pinAssaySensor, INPUT);
  pinMode(pinControlSensor, INPUT);
  pinMode(pinButtonRun, INPUT);
  
  pinMode(pinButtonLight, OUTPUT);
  pinMode(pinSolenoid, OUTPUT);
  
  stateButtonRun = digitalRead(pinButtonRun);
  digitalWrite(pinButtonLight, LOW);

    // initialize analyzer
  Serial.println("Initializing analyzer");
  solenoid_out();
  reset_x_stage();
}

void reset_x_stage() {
  Serial.println("Resetting X stage");
  while(digitalRead(pinBackLimitSwitch) == LOW) {
    motor.step(1, BACKWARD, DOUBLE);
  }
  motor.release();
  delay(1000);
}

void loop() {
  int rVal, rVal2;
  
  rVal = digitalRead(pinButtonRun);
  delay(DEBOUNCE);
  rVal2 = digitalRead(pinButtonRun);
  
  if (rVal == rVal2) {
    if (rVal != stateButtonRun) {
      if (rVal == HIGH) {
        Serial.println("Run button pushed");
        brevitest_run();
      }
      else {
        stateButtonRun = rVal;
      }
    }
  }
}

void brevitest_run() {
  bt_setup();

  bt_raster_first_well();
  bt_move_to_next_well();
  
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
  reset_x_stage();
  bt_move_to_first_well();
  digitalWrite(pinButtonLight, HIGH);
}

void bt_move_to_first_well() {
  double mm = 4.0;
  int steps = int(round(mm / mmPerStep));
  motor.step(steps, FORWARD, DOUBLE);
}

void solenoid_out() {
    Serial.println("Shifting out...");
    solenoid.run(RELEASE);
}

void solenoid_in() {
    Serial.println("Shifting in...");
    solenoid.run(FORWARD);
}

void raster_well(double mm) {
  int number_of_rasters = int(round(mm / mmPerRaster));
  
  for (int i = 0; i < number_of_rasters; i += 1) {
    motor.step(stepsPerRaster, FORWARD, SINGLE);
    delay(solenoidDelayAfterRaster);
    solenoid_in();
    delay(solenoidDelay);
    solenoid_out();
  }
}

void move_mm(double mm) {
  int steps = int(round(abs(mm) / mmPerStep));
  if (mm > 0) {
    motor.step(steps, FORWARD, SINGLE);
  }
  else {
    motor.step(steps, BACKWARD, SINGLE);
  }
}

void bt_raster_first_well() {
  raster_well(12.0);
}

void bt_move_to_next_well() {
  move_mm(4.0);
}

void bt_raster_well() {
  raster_well(6.0);
}

void bt_raster_last_well() {
  raster_well(8.0);
}

void bt_position_sensors() {
  move_mm(20.0);
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
  motor.release();
  digitalWrite(pinButtonLight, LOW);
}
