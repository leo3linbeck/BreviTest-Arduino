void sample_sensor(SensorSample *sample, SensorReading *reading, int count);
void get_one_sensor_reading(SensorReading *baseline);
void average_sample(SensorSample *sample, SensorReading *reading);
void print_sensor_reading(SensorReading *reading);
void print_sensor_reading_diff(SensorReadingDiff *diff);
void get_baseline_sensor_reading(int pin, SensorReading *baseline);
void normalize_sensor_reading(SensorReading *reading);
void diff_reading_from_baseline(SensorReadingDiff *diff, SensorReading *reading, SensorReading *baseline);

SensorReading assayBaseline, controlBaseline;
int reading_count = 0;

void sensor_setup() {
  Serial.println(F("Initializing sensors"));
  
//  digitalWrite(pinAssaySensor, HIGH);
//  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);
//  Serial.println(F("Starting up assay sensor"));
//  sensor.begin();
//  delay(100);
//  Serial.println(F("Turning off assay sensor LED"));
//  sensor.setInterrupt(true);
//  digitalWrite(pinAssaySensor, LOW);
//  
//  digitalWrite(pinControlSensor, HIGH);
//  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);
//  Serial.println(F("Starting up control sensor"));
//  sensor.begin();
//  delay(100);
//  Serial.println(F("Turning off control sensor LED"));
//  sensor.setInterrupt(true);
//
//  digitalWrite(pinControlSensor, LOW);

//  digitalWrite(pinAssaySensor, LOW);
//  digitalWrite(pinControlSensor, LOW);

//  Serial.println(F("Setting assay sensor"));
//  digitalWrite(pinControlSensor, LOW);
//  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);
//  sensor.begin();
//  delay(100);
////  sensor.setInterrupt(true);
//  digitalWrite(pinControlSensor, HIGH);
//  
//  Serial.println(F("Setting control sensor"));
//  digitalWrite(pinAssaySensor, LOW);
//  sensor.begin();
//  delay(100);
////  sensor.setInterrupt(true);
//  digitalWrite(pinAssaySensor, HIGH);
}

void normalize_sensor_reading(SensorReading *reading) {
  float d = float(reading->lux);
  reading->norm.r = float(reading->raw.r) / d;
  reading->norm.g = float(reading->raw.g) / d;
  reading->norm.b = float(reading->raw.b) / d;
}
  
void get_baseline_sensor_reading(int pin, SensorReading *baseline) {
    SensorSample sample;
    
    digitalWrite(pin, HIGH);
    delay(100);
    sensor.setInterrupt(false);
    sample_sensor(&sample, baseline, 20);
    normalize_sensor_reading(baseline);
    print_sensor_reading(baseline);
    sensor.setInterrupt(true);

//    if (pin == pinAssaySensor) {
//      digitalWrite(pinControlSensor, HIGH);
//    }
//    else {
//      digitalWrite(pinAssaySensor, HIGH);
//    }
    digitalWrite(pin, LOW);
}

void average_sample(SensorSample *sample, SensorReading *reading) {
  long c = sample->count;
  reading->raw.r = sample->r / c;
  reading->raw.g = sample->g / c;
  reading->raw.b = sample->b / c;
  reading->raw.c = sample->c / c;
  reading->colorTemp = sensor.calculateColorTemperature(reading->raw.r, reading->raw.g, reading->raw.b);
  reading->lux = sensor.calculateLux(reading->raw.r, reading->raw.g, reading->raw.b);
}

void sample_sensor(SensorSample *sample, SensorReading *reading, int count) {
  RawSensorReading data;

  for (int i = 0; i < 3; i += 1) {  // throw away first few readings
    sensor.getRawData(&data.r, &data.g, &data.b, &data.c);
  }
  
  for (sample->count = 0; sample->count < count; sample->count += 1) {
    sensor.getRawData(&data.r, &data.g, &data.b, &data.c);
    sample->r += data.r;
    sample->g += data.g;
    sample->b += data.b;
    sample->c += data.c;
  }

  average_sample(sample, reading);
}

void print_sensor_reading(SensorReading *reading) {
  Serial.print("\tRaw->");
  Serial.print("\tRed: ");
  Serial.print(reading->raw.r);
  Serial.print("\tGreen: ");
  Serial.print(reading->raw.g);
  Serial.print("\tBlue: ");
  Serial.print(reading->raw.b);
  Serial.print("\tClear: ");
  Serial.print(reading->raw.c);
  Serial.print("\tTemp : ");
  Serial.print(reading->colorTemp);
  Serial.print("\tLux: ");
  Serial.println(reading->lux);
  
//  Serial.print("Norm-> ");
//  Serial.print("  Red: ");
//  Serial.print(reading->norm.r);
//  Serial.print("  Green: ");
//  Serial.print(reading->norm.g);
//  Serial.print("  Blue: ");
//  Serial.println(reading->norm.b);
}

void print_sensor_reading_diff(SensorReadingDiff *diff) {
  Serial.print("Diff Raw-> ");
  Serial.print("  Red: ");
  Serial.print(diff->raw.r);
  Serial.print("  Green: ");
  Serial.print(diff->raw.g);
  Serial.print("  Blue: ");
  Serial.print(diff->raw.b);
  Serial.print("  Clear: ");
  Serial.println(diff->raw.c);

  Serial.print("Diff Norm-> ");
  Serial.print("  Red: ");
  Serial.print(diff->norm.r);
  Serial.print("  Green: ");
  Serial.print(diff->norm.g);
  Serial.print("  Blue: ");
  Serial.println(diff->norm.b);
}

void sensor_loop() {
//  reading_count += 1;
//  Serial.print(reading_count);
//  Serial.print(F("\tAssay"));
//  digitalWrite(pinControlSensor, LOW);
//  delay(100);
//  get_one_sensor_reading(&assayBaseline);
//  digitalWrite(pinControlSensor, HIGH);
//  delay(100);
//  Serial.print(reading_count);
//  Serial.print(F("\tControl"));
//  digitalWrite(pinAssaySensor, LOW);
//  delay(100);
//  get_one_sensor_reading(&controlBaseline);
//  digitalWrite(pinAssaySensor, HIGH);
//  delay(100);
}

void diff_reading_from_baseline(SensorReadingDiff *diff, SensorReading *reading, SensorReading *baseline) {
  diff->raw.r = long(reading->raw.r) - long(baseline->raw.r);
  diff->raw.g = long(reading->raw.g) - long(baseline->raw.g);
  diff->raw.b = long(reading->raw.b) - long(baseline->raw.b);
  diff->raw.c = long(reading->raw.c) - long(baseline->raw.c);

  diff->norm.r = reading->norm.r - baseline->norm.r;
  diff->norm.g = reading->norm.g - baseline->norm.g;
  diff->norm.b = reading->norm.b - baseline->norm.b;
}

void get_one_sensor_reading(SensorReading *baseline) {
  SensorReadingDiff diff;
  SensorReading data;
  
  sensor.getRawData(&data.raw.r, &data.raw.g, &data.raw.b, &data.raw.c);
  data.colorTemp = sensor.calculateColorTemperature(data.raw.r, data.raw.g, data.raw.b);
  data.lux = sensor.calculateLux(data.raw.r, data.raw.g, data.raw.b);
//  normalize_sensor_reading(&data);
  print_sensor_reading(&data);
  
//  diff_reading_from_baseline(&diff, &data, baseline);
//  print_sensor_reading_diff(&diff);
}

void get_sensor_readings() {
  
}

void calibrate_sensors() {
  Serial.println(F("Assay sensor baseline:"));
//  get_baseline_sensor_reading(pinAssaySensor, &assayBaseline);
  Serial.println(F("Control sensor baseline:"));
//  get_baseline_sensor_reading(pinControlSensor, &controlBaseline);
  Serial.println(F("Baseline readings complete."));
  move_mm(50.0);
}
