void device_setup() {
    // initialize pins
  pinMode(pinFrontLimitSwitch, INPUT);
  pinMode(pinBackLimitSwitch, INPUT);
  
  Serial.println(F("Initializing motor shield"));
  AFMS.begin();
  Serial.println(F("Initializing motor"));
  motor->setSpeed(rpm);
  Serial.println(F("Initializing solenoid"));
  solenoid->setSpeed(solenoid_power);
  
  reset_x_stage();
}

void reset_x_stage() {
  Serial.println(F("Resetting X stage"));
  move_steps(30000, STEP_BACKWARD, DOUBLE);
  motor->release();
  delay(1000);
}

void run_brevitest() {
  Serial.println(F("Running BreviTest..."));
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
  reset_x_stage();
}

void bt_move_to_second_well() {
  Serial.println(F("Moving to second well"));
  move_mm(15.0);
}

void bt_move_back_to_analyte_well() {
  Serial.println(F("Moving back to analyte well"));
  move_mm(-16.0);
}

void solenoid_out() {
    Serial.println(F("Shifting out..."));
    solenoid->run(RELEASE);
}

void solenoid_in() {
    Serial.println(F("Shifting in..."));
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

void move_mm(double mm) {
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

void bt_raster_first_well() {
  Serial.println(F("Rastering first well well"));
  raster_well(6.0);
}

void bt_move_to_next_well() {
  Serial.println(F("Moving to next well"));
  move_mm(4.0);
}

void bt_skip_well() {
  Serial.println(F("Skipping well"));
  move_mm(10.0);
}

void bt_raster_well() {
  Serial.println(F("Rastering well"));
  raster_well(6.0);
}

void bt_raster_last_well() {
  Serial.println(F("Rastering last well"));
  raster_well(8.0);
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
