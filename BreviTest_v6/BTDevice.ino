// NOTE: FORWARD and BACKWARD are reversed for the stepper motor!!!

#define MOTOR_CHANNEL 1
#define SOLENOID_CHANNEL 3

#define PIN_START_LIMIT_SWITCH 5
#define PIN_FINISH_LIMIT_SWITCH 6

#define STEP_FORWARD BACKWARD
#define STEP_BACKWARD FORWARD

#define STEPS_PER_RASTER 100  // 0.5mm per raster
#define STEPS_PER_ROTATION 200  // 1.0mm per rotation
#define RPM 200

#define NUMBER_OF_WELLS 6   // does not include microbead and color wells
#define NUMBER_OF_SENSOR_READINGS 10
#define DELAY_BETWEEN_SENSOR_READINGS 500

#define SOLENOID_SUSTAIN_POWER 15
#define SOLENOID_SURGE_POWER 255
#define SOLENOID_SURGE_PERIOD 80

void device_setup() {
    // initialize pins
  pinMode(PIN_START_LIMIT_SWITCH, INPUT);
  pinMode(PIN_FINISH_LIMIT_SWITCH, INPUT);
  
//  Serial.println(F("Initializing motor shield"));
  AFMS = Adafruit_MotorShield();
  AFMS.begin();
//  Serial.println(F("Initializing motor"));
  motor = AFMS.getStepper(STEPS_PER_ROTATION, MOTOR_CHANNEL);
  motor->setSpeed(RPM);
  
//  Serial.println(F("Initializing solenoid"));
  solenoid = AFMS.getMotor(SOLENOID_CHANNEL);
  
  solenoid_in();
  delay(500);
  solenoid_out();
  
  reset_x_stage();  
}

void move_steps(int steps, int dir, int rate) {
//  Serial.print(F("Moving "));
//  Serial.print(steps);
//  Serial.println(F(" steps"));
  for (int i = 0; i < steps; i += 1) {
    if (dir == STEP_FORWARD && digitalRead(PIN_FINISH_LIMIT_SWITCH) == HIGH) {
//      Serial.println(F("Front limit switch tripped"));
      return;
    }
    if (dir == STEP_BACKWARD && digitalRead(PIN_START_LIMIT_SWITCH) == HIGH) {
//      Serial.println(F("Back limit switch tripped"));
      return;
    }
    motor->step(1, dir, rate);
  }
}

void reset_x_stage() {
//  Serial.println(F("Resetting X stage"));
  move_steps(30000, STEP_BACKWARD, SINGLE);
  motor->release();
}

void solenoid_out() {
//  Serial.println(F("Shifting out..."));
  solenoid->run(RELEASE);
}

void solenoid_in() {
//  Serial.println(F("Shifting in..."));
  solenoid->setSpeed(SOLENOID_SURGE_POWER);
  solenoid->run(FORWARD);
  delay(SOLENOID_SURGE_PERIOD);
  solenoid->setSpeed(SOLENOID_SUSTAIN_POWER);
}

void run_brevitest() {
  Serial.println(F("Running BreviTest..."));
  
  reset_x_stage();
  move_steps(3000, STEP_FORWARD, SINGLE);
  delay(10000); // put in card
  move_steps(2000, STEP_FORWARD, SINGLE);
  raster_well(10); // sample well
  bt_move_to_next_well(); 
  raster_well(10); // buffer well
  bt_move_to_next_well();   
  raster_well(10); // cortisol-hrp well
  bt_read_sensors();
  bt_move_to_next_well();
  raster_well(10); // buffer well
  bt_move_to_next_well();
  raster_well(14); // tmb substrate well
  move_steps(1400, STEP_BACKWARD, SINGLE);
  raster_well(14);
  bt_position_sensors();
  bt_read_sensors();

  bt_cleanup();
  
  Serial.println(F("BreviTest run complete."));
}

void bt_move_to_second_well() {
//  Serial.println(F("Moving to second well"));
  move_steps(3000, STEP_FORWARD, SINGLE);
}

void raster_well(int number_of_rasters) {
  for (int i = 0; i < number_of_rasters; i += 1) {
    move_steps(STEPS_PER_RASTER, STEP_FORWARD, SINGLE);
    if (i < 1) {
      delay(500);
      solenoid_in();
      delay(2200);
      solenoid_out();
    }
    else {
      for (int k = 0; k < 4; k += 1) {
        delay(250);
        solenoid_in();
        delay(700);
        solenoid_out();
      }
    }
  }
  delay(4000);
}
void bt_move_to_next_well() {
//  Serial.println(F("Moving to next well"));
  move_steps(1000, STEP_FORWARD, SINGLE);
}

void bt_position_sensors() {
//  Serial.println(F("Moving sensors into position"));
  move_steps(4400, STEP_BACKWARD, SINGLE);
}

void bt_read_sensors() {
//  Serial.println(F("Reading sensors"));
  get_sensor_readings();
}

void bt_cleanup() {
//  Serial.println(F("Finishing up"));
  solenoid_out();
  reset_x_stage();
  motor->release();
}
