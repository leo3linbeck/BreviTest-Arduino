#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define pinAssaySensor 4
#define pinControlSensor 2

Adafruit_TCS34725 sensor;

struct RawSensorReading {
  uint16_t r;
  uint16_t g;
  uint16_t b;
  uint16_t c;
};

struct SensorReading {
  RawSensorReading raw;
  struct norm {
    float r;
    float g;
    float b;
  } norm;
  int colorTemp;
  int lux;
  
  SensorReading() {
    raw.r = 0;
    raw.g = 0;
    raw.b = 0;
    raw.c = 0;
    norm.r = 0;
    norm.g = 0;
    norm.b = 0;
    colorTemp = 0;
    lux = 0;
  }
};

struct SensorReadingDiff {
  struct raw {
    int r;
    int g;
    int b;
    int c;
  } raw;
  
  struct norm {
    float r;
    float g;
    float b;
  } norm;
  
  SensorReadingDiff() {
    raw.r = 0;
    raw.g = 0;
    raw.b = 0;
    raw.c = 0;
    
    norm.r = 0;
    norm.g = 0;
    norm.b = 0;
  }
};

struct SensorSample {
  long r;
  long g;
  long b;
  long c;
  int count;
  
  SensorSample() {
    r = 0;
    g = 0;
    b = 0;
    c = 0;
    count = 0;
  }
};

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

void setup() {
  Serial.begin(9600);
  Serial.println(F("Serial port started"));
  delay(500);
  
  pinMode(pinAssaySensor, OUTPUT);
  pinMode(pinControlSensor, OUTPUT);
  
  digitalWrite(pinAssaySensor, HIGH);
  digitalWrite(pinControlSensor, HIGH);

  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_4X);
  sensor.begin();
  
  digitalWrite(pinAssaySensor, LOW);
  digitalWrite(pinControlSensor, LOW);
  
  Serial.println(F("Assay sensor baseline:"));
  get_baseline_sensor_reading(pinAssaySensor, &assayBaseline);
  Serial.println(F("Control sensor baseline:"));
  get_baseline_sensor_reading(pinControlSensor, &controlBaseline);
  Serial.println(F("Baseline readings complete."));
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
    
    sample_sensor(&sample, baseline, 20);
    normalize_sensor_reading(baseline);
    print_sensor_reading(baseline);
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

void loop() {
  reading_count += 1;
  Serial.print(reading_count);
  Serial.print(F("\tAssay"));
  digitalWrite(pinAssaySensor, HIGH);
  delay(100);
  get_one_sensor_reading(&assayBaseline);
  digitalWrite(pinAssaySensor, LOW);
  delay(100);
  Serial.print(reading_count);
  Serial.print(F("\tControl"));
  digitalWrite(pinControlSensor, HIGH);
  delay(100);
  get_one_sensor_reading(&controlBaseline);
  digitalWrite(pinControlSensor, LOW);
  delay(100);
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
