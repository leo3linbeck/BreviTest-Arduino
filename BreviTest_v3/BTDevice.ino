void device_setup() {
    // initialize pins
  pinMode(pinFrontLimitSwitch, INPUT);
  pinMode(pinBackLimitSwitch, INPUT);
//  Serial.println(digitalRead(pinFrontLimitSwitch));
//  Serial.println(digitalRead(pinBackLimitSwitch));
  
  Serial.println(F("Initializing motor shield"));
  AFMS = Adafruit_MotorShield();
  AFMS.begin();
  Serial.println(F("Initializing motor"));
  motor = AFMS.getStepper(stepsPerRotation, channelMotor);
  motor->setSpeed(rpm);
  Serial.println(F("Initializing solenoid"));
  solenoid = AFMS.getMotor(channelSolenoid);
  solenoid->setSpeed(solenoid_power);
  
  solenoid_in();
  delay(500);
  solenoid_out();
  
  reset_x_stage();  
}

void move_steps(int steps, int dir, int rate) {
  Serial.print(F("Moving "));
  Serial.print(steps);
  Serial.println(F(" steps"));
  for (int i = 0; i < steps; i += 1) {
    if (dir == STEP_FORWARD && digitalRead(pinFrontLimitSwitch) == HIGH) {
      Serial.println(F("Front limit switch tripped"));
      return;
    }
    if (dir == STEP_BACKWARD && digitalRead(pinBackLimitSwitch) == HIGH) {
      Serial.println(F("Back limit switch tripped"));
      return;
    }
    motor->step(1, dir, rate);
  }
}

void move_mm(float mm) {
  int steps = int(round(abs(mm) / mmPerStep));
  Serial.print(F("Steps: "));
  Serial.println(steps);
  if (mm > 0) {
    move_steps(steps, STEP_FORWARD, SINGLE);
  }
  else {
    move_steps(steps, STEP_BACKWARD, SINGLE);
  }
}

void reset_x_stage() {
  Serial.println(F("Resetting X stage"));
  move_steps(30000, STEP_BACKWARD, SINGLE);
  motor->release();
  delay(1000);
}

void solenoid_out() {
    Serial.println(F("Shifting out..."));
    solenoid->run(RELEASE);
}

void solenoid_in() {
    Serial.println(F("Shifting in..."));
    solenoid->run(FORWARD);
}

void run_brevitest() {
  Serial.println(F("Running BreviTest..."));
  reset_x_stage();

  bt_move_to_second_well();
  for (int i = 0; i < number_of_wells; i += 1) {
    raster_well(5.0);
    bt_move_to_next_well();
  }
  
  raster_well(7.0);
  bt_position_sensors();
  bt_read_sensors();

  bt_cleanup();
}

void bt_move_to_second_well() {
  Serial.println(F("Moving to second well"));
  move_mm(15.0);
}

void raster_well(float mm) {
  int number_of_rasters = int(round(mm/ mmPerRaster));
  for (int i = 0; i < number_of_rasters; i += 1) {
    move_mm(mmPerRaster);
    if (i < 5) {
      delay(500);
      solenoid_in();
      delay(1200);
      solenoid_out();
    }
    else {
      delay(250);
      solenoid_in();
      delay(700);
      solenoid_out();
    }
  }
  delay(4000);
}
void bt_move_to_next_well() {
  Serial.println(F("Moving to next well"));
  move_mm(5.0);
}

void bt_position_sensors() {
  Serial.println(F("Moving sensors into position"));
  move_mm(-24.0);
}

void bt_read_sensors() {
  Serial.println(F("Reading sensors"));
  get_sensor_readings();
}

void bt_cleanup() {
  Serial.println(F("Finishing up"));
  solenoid_out();
  reset_x_stage();
  motor->release();
}
