void sample_sensor(Adafruit_TCS34725 *sensor, SensorSample *sample, SensorReading *reading, int count);
void get_one_sensor_reading(Adafruit_TCS34725 *sensor, SensorReading *baseline);
void average_sample(Adafruit_TCS34725 *sensor, SensorSample *sample, SensorReading *reading);
void print_sensor_reading(SensorReading *reading);
void print_sensor_reading_diff(SensorReadingDiff *diff);
void get_baseline_sensor_reading(Adafruit_TCS34725 *sensor, SensorReading *baseline);
void normalize_sensor_reading(SensorReading *reading);
void diff_reading_from_baseline(SensorReadingDiff *diff, SensorReading *reading, SensorReading *baseline);

Adafruit_TCS34725 assaySensor;
Adafruit_TCS34725 controlSensor;
SensorReading assayBaseline;
SensorReading controlBaseline;

void sensor_setup() {
  Serial.println(F("Starting up assay sensor"));
  assaySensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X, pinAssaySensorSDA, pinAssaySensorSCL);
  assaySensor.begin();
  delay(1000);
  assaySensor.endReading();
  delay(1000);

  Serial.println(F("Starting up control sensor"));
  controlSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X, pinControlSensorSDA, pinControlSensorSCL);
  controlSensor.begin();
  delay(1000);
  controlSensor.endReading();
  delay(1000);
}

void test_sensors() {
  reset_x_stage();
  move_mm(11.0);
  Serial.println(F("Reading 1"));
  get_sensor_readings();
  move_mm(11.0);
  Serial.println(F("Reading 2"));
  get_sensor_readings();
  move_mm(11.0);
  Serial.println(F("Reading 3"));
  get_sensor_readings();
  move_mm(11.0);
  Serial.println(F("Reading 4"));
  get_sensor_readings();
  move_mm(11.0);
  Serial.println(F("Reading 5"));
  get_sensor_readings();
  reset_x_stage();
}

//void normalize_sensor_reading(SensorReading *reading) {
//  float d = float(reading->lux);
//  reading->norm.r = float(reading->raw.r) / d;
//  reading->norm.g = float(reading->raw.g) / d;
//  reading->norm.b = float(reading->raw.b) / d;
//}
  
//void get_baseline_sensor_reading(Adafruit_TCS34725 *sensor, SensorReading *baseline) {
//    SensorSample sample;
//    
//    sensor->setInterrupt(false);
//    delay(500);
//    sample_sensor(sensor, &sample, baseline, 20);
//    normalize_sensor_reading(baseline);
//    print_sensor_reading(baseline);
//    sensor->setInterrupt(true);
//}

//void average_sample(Adafruit_TCS34725 *sensor, SensorSample *sample, SensorReading *reading) {
//  long c = 1;
//  reading->raw.r = sample->r / c;
//  reading->raw.g = sample->g / c;
//  reading->raw.b = sample->b / c;
//  reading->raw.c = sample->c / c;
//  reading->colorTemp = sensor->calculateColorTemperature(reading->raw.r, reading->raw.g, reading->raw.b);
//  reading->lux = sensor->calculateLux(reading->raw.r, reading->raw.g, reading->raw.b);
//}

//void sample_sensor(Adafruit_TCS34725 *sensor, SensorSample *sample, SensorReading *reading, int count) {
//  RawSensorReading data;
//
//  for (int i = 0; i < 1; i += 1) {  // throw away first few readings
//    sensor->getRawData(&data.r, &data.g, &data.b, &data.c);
//  }
//  
//  for (sample->count = 0; sample->count < count; sample->count += 1) {
//    sensor->getRawData(&data.r, &data.g, &data.b, &data.c);
//    sample->r += data.r;
//    sample->g += data.g;
//    sample->b += data.b;
//    sample->c += data.c;
//  }
//
//  average_sample(sensor, sample, reading);
//}

void print_sensor_reading(SensorReading *reading) {
  Serial.print(F("\tRaw->"));
  Serial.print(F("  Count: "));
  Serial.print(reading->count);
  Serial.print(F("  \tRed: "));
  Serial.print(reading->raw.r);
  Serial.print(F("  \tGreen: "));
  Serial.print(reading->raw.g);
  Serial.print(F("  \tBlue: "));
  Serial.print(reading->raw.b);
  Serial.print(F("  \tClear: "));
  Serial.print(reading->raw.c);
  Serial.print(F("  \tTemp : "));
  Serial.print(reading->colorTemp);
  Serial.print(F("  \tLux: "));
  Serial.println(reading->lux);
  
//  Serial.print(F("Norm-> "));
//  Serial.print(F("  Red: "));
//  Serial.print(reading->norm.r);
//  Serial.print(F("  Green: "));
//  Serial.print(reading->norm.g);
//  Serial.print(F("  Blue: "));
//  Serial.println(reading->norm.b);
}

//void print_sensor_reading_diff(SensorReadingDiff *diff) {
//  Serial.print(F("Diff Raw-> "));
//  Serial.print(F("  Red: "));
//  Serial.print(diff->raw.r);
//  Serial.print(F("  Green: "));
//  Serial.print(diff->raw.g);
//  Serial.print(F("  Blue: "));
//  Serial.print(diff->raw.b);
//  Serial.print(F("  Clear: "));
//  Serial.println(diff->raw.c);
//
//  Serial.print(F("Diff Norm-> "));
//  Serial.print(F("  Red: "));
//  Serial.print(diff->norm.r);
//  Serial.print(F("  Green: "));
//  Serial.print(diff->norm.g);
//  Serial.print(F("  Blue: "));
//  Serial.println(diff->norm.b);
//}

void sensor_loop() {
//  reading_count += 1;
//  Serial.print(reading_count);
//  Serial.print(F("\tAssay"));
//  digitalWrite(pinControlSensor, LOW);
//  delay(100);
//  get_one_sensor_reading(&assaySensor, &assayBaseline);
//  digitalWrite(pinControlSensor, HIGH);
//  delay(100);
//  Serial.print(reading_count);
//  Serial.print(F("\tControl"));
//  digitalWrite(pinAssaySensor, LOW);
//  delay(100);
//  get_one_sensor_reading(&controlSensor, &controlBaseline);
//  digitalWrite(pinAssaySensor, HIGH);
//  delay(100);
}

//void diff_reading_from_baseline(SensorReadingDiff *diff, SensorReading *reading, SensorReading *baseline) {
//  diff->raw.r = long(reading->raw.r) - long(baseline->raw.r);
//  diff->raw.g = long(reading->raw.g) - long(baseline->raw.g);
//  diff->raw.b = long(reading->raw.b) - long(baseline->raw.b);
//  diff->raw.c = long(reading->raw.c) - long(baseline->raw.c);
//
//  diff->norm.r = reading->norm.r - baseline->norm.r;
//  diff->norm.g = reading->norm.g - baseline->norm.g;
//  diff->norm.b = reading->norm.b - baseline->norm.b;
//}

void get_one_sensor_reading(Adafruit_TCS34725 *sensor, SensorReading *baseline) {
  SensorReadingDiff diff;
  SensorReading data;
  int oldColorTemp;
  bool useLED = true;
  
  sensor->startReading(useLED);
  do {
    data.count++;
    oldColorTemp = data.colorTemp;
    sensor->getRawData(&data.raw.r, &data.raw.g, &data.raw.b, &data.raw.c);
    data.colorTemp = sensor->calculateColorTemperature(data.raw.r, data.raw.g, data.raw.b);
    data.lux = sensor->calculateLux(data.raw.r, data.raw.g, data.raw.b);
  } while (data.count < 10 || (data.count <= 50 && oldColorTemp != data.colorTemp));
  sensor->endReading();
//  normalize_sensor_reading(&data);
  print_sensor_reading(&data);
  
//  diff_reading_from_baseline(&diff, &data, baseline);
//  print_sensor_reading_diff(&diff);
}

void get_sensor_readings() {
  Serial.print(F("Assay:  "));
  get_one_sensor_reading(&assaySensor, &assayBaseline);
  
  delay(1000);
  
  Serial.print(F("Control:"));
  get_one_sensor_reading(&controlSensor, &controlBaseline);
}

//void calibrate_sensors() {
//  Serial.print(F("Assay:  "));
//  get_baseline_sensor_reading(&assaySensor, &assayBaseline);
//  Serial.print(F("Control:"));
//  get_baseline_sensor_reading(&controlSensor, &controlBaseline);
//  Serial.println(F("Baseline readings complete."));
////  move_mm(50.0);
//}
