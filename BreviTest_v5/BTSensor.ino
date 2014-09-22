#define PIN_ASSAY_SDA A0
#define PIN_ASSAY_SCL A1
#define PIN_CONTROL_SDA A2
#define PIN_CONTROL_SCL A3

void sensor_setup() {
//  Serial.println(F("Starting up assay sensor"));
  assaySensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X, PIN_ASSAY_SDA, PIN_ASSAY_SCL);
  assaySensor.begin();
  delay(1000);
  assaySensor.endReading();
  delay(1000);

//  Serial.println(F("Starting up control sensor"));
  controlSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X, PIN_CONTROL_SDA, PIN_CONTROL_SCL);
  controlSensor.begin();
  delay(1000);
  controlSensor.endReading();
  delay(1000);
}

void test_sensors() {
  reset_x_stage();
  move_steps(2200, STEP_FORWARD, SINGLE);
//  Serial.println(F("Reading 1"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
//  Serial.println(F("Reading 2"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
//  Serial.println(F("Reading 3"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
//  Serial.println(F("Reading 4"));
  get_sensor_readings();
  move_steps(2200, STEP_FORWARD, SINGLE);
//  Serial.println(F("Reading 5"));
  get_sensor_readings();
  reset_x_stage();
}

void get_one_sensor_reading(Adafruit_TCS34725 *sensor) {
  uint16_t r, g, b, c;
  int count = 0;
  int colorTemp = 0;
  int oldColorTemp, lux;
  bool useLED = true;
  
  sensor->startReading(useLED);
  do {
    count++;
    oldColorTemp = colorTemp;
    sensor->getRawData(&r, &g, &b, &c);
    colorTemp = sensor->calculateColorTemperature(r, g, b);
    lux = sensor->calculateLux(r, g, b);
  } while (count < 10 || (count <= 50 && oldColorTemp != colorTemp));
  sensor->endReading();
}

void get_sensor_readings() {
//  Serial.print(F("Assay:  "));
  get_one_sensor_reading(&assaySensor);
  
  delay(1000);
  
//  Serial.print(F("Control:"));
  get_one_sensor_reading(&controlSensor);
}

