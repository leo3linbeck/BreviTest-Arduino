#define PIN_ASSAY_SDA A0
#define PIN_ASSAY_SCL A1
#define PIN_CONTROL_SDA A2
#define PIN_CONTROL_SCL A3
#define COLOR_TOLERANCE 5
#define SENSOR_STARTUP_DELAY 60000
#define SENSOR_READING_COUNT 50
#define SENSOR_READING_DELAY 10

void sensor_setup() {
  bool useLED = true;
  
//  Serial.println(F("Starting up assay sensor"));
  assaySensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, PIN_ASSAY_SDA, PIN_ASSAY_SCL);
  assaySensor.begin();
  assaySensor.startReading(useLED);

//  Serial.println(F("Starting up control sensor"));
  controlSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, PIN_CONTROL_SDA, PIN_CONTROL_SCL);
  controlSensor.begin();
  controlSensor.startReading(useLED);
//  Serial.println(F("Sensors warming up. This takes about one minute..."));
//
//  delay(SENSOR_STARTUP_DELAY);
//
//  Serial.println(F("Warmup complete."));
//  Serial.println(F("TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X"));
}

void test_sensors() {
  reset_x_stage();
  move_steps(2200, STEP_FORWARD, SINGLE);
  Serial.println(F("Reading 1"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
  Serial.println(F("Reading 2"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
  Serial.println(F("Reading 3"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
  Serial.println(F("Reading 4"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
  Serial.println(F("Reading 5"));
  get_sensor_readings();
  reset_x_stage();
}

void get_sensor_readings() {
  uint16_t assayR, assayG, assayB, assayC;
  uint16_t controlR, controlG, controlB, controlC;
  unsigned long t;
  
  motor->release();
  delay(100);
  for (int i = 0; i < SENSOR_READING_COUNT; i += 1) {
    t = millis() - start_time;
    assaySensor.getRawData(&assayR, &assayG, &assayB, &assayC);
    controlSensor.getRawData(&controlR, &controlG, &controlB, &controlC);
    Serial.print("n:\t");
    Serial.print(i);
    Serial.print("\tt:\t");
    Serial.print(t);
    Serial.print("\tAssay:\tR:\t");
    Serial.print(assayR);
    Serial.print("\tG:\t");
    Serial.print(assayG);
    Serial.print("\tB:\t");
    Serial.print(assayB);
    Serial.print("\tC:\t");
    Serial.print(assayC);
    Serial.print("\tControl:\tR:\t");
    Serial.print(controlR);
    Serial.print("\tG:\t");
    Serial.print(controlG);
    Serial.print("\tB:\t");
    Serial.print(controlB);
    Serial.print("\tC:\t");
    Serial.println(controlC);
    delay(SENSOR_READING_DELAY);
  }
}


