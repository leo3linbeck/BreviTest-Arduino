void sensor_setup() {
  Serial.println(F("Setting up sensors..."));
  Adafruit_TCS34725 sensor;
  
  pinMode(pinAssaySensorVin, OUTPUT);
  pinMode(pinAssaySensorLED, OUTPUT);

  Serial.println(F("Turning on assay sensor..."));
  digitalWrite(pinAssaySensorVin, HIGH);
  Serial.println(F("Turning on assay sensor LED..."));
  digitalWrite(pinAssaySensorLED, HIGH);
  delay(1000);
  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_16X);
  if (sensor.begin()) {
    Serial.println(F("Assay sensor successfully started up"));
    Serial.println(F("Disabling assay sensor..."));
    sensor.disable();
  }
  else {
    Serial.println(F("Assay sensor failed to start up"));
  }
  Serial.println(F("Turning off assay sensor LED..."));
  digitalWrite(pinAssaySensorLED, LOW);
  
  delay(1000);
 
  pinMode(pinControlSensorVin, OUTPUT);
  pinMode(pinControlSensorLED, OUTPUT);
  
  Serial.println(F("Turning on assay sensor..."));
  digitalWrite(pinControlSensorVin, HIGH);
  Serial.println(F("Turning on assay sensor LED..."));
  digitalWrite(pinControlSensorLED, HIGH);
  delay(1000);
  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_16X);
  if (sensor.begin()) {
    Serial.println(F("Assay sensor successfully started up"));
    Serial.println(F("Disabling assay sensor..."));
    sensor.disable();
  }
  else {
    Serial.println(F("Assay sensor failed to start up"));
  }
  Serial.println(F("Turning off assay sensor LED..."));
  digitalWrite(pinControlSensorLED, LOW);
}

void read_sensors() {
  Serial.println(F("Reading sensors..."));
  reset_x_stage();
  move_mm(60.0);
  get_sensor_readings();
}

void get_sensor_readings() {
  int colorTemp = 0;
  int lastColorTemp = 0;
  Adafruit_TCS34725 sensor;
  
  digitalWrite(pinControlSensorVin, LOW);
  analogWrite(pinAssaySensorLED, 1000);
  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_16X);
  delay(500);
  sensor.enable();
  delay(500);
  Serial.println("Reading assay sensor: ");
  for (int i = 0; i < 20; i += 1) {
    colorTemp = get_one_sensor_reading(sensor);
    if (i > 3 && colorTemp == lastColorTemp) {
      break;
    }
    lastColorTemp = colorTemp;
    delay(500);
  }
  analogWrite(pinAssaySensorLED, 0);
  sensor.disable();
  digitalWrite(pinControlSensorVin, HIGH);
  Serial.println("Assay sensor reading complete");
  
  digitalWrite(pinAssaySensorVin, LOW);
  analogWrite(pinControlSensorLED, 1000);
  sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_16X);
  delay(500);
  sensor.enable();
  delay(500);
  Serial.println("Reading control sensor: ");
  for (int i = 0; i < 20; i += 1) {
    colorTemp = get_one_sensor_reading(sensor);
    if (i > 3 && colorTemp == lastColorTemp) {
      break;
    }
    lastColorTemp = colorTemp;
    delay(500);
  }
  analogWrite(pinControlSensorLED, 0);
  sensor.disable();
  digitalWrite(pinAssaySensorVin, HIGH);
  Serial.println("Control sensor reading complete");
}

int get_one_sensor_reading(Adafruit_TCS34725 sensor) {
  uint16_t r, g, b, c;
  
  sensor.getRawData(&r, &g, &b, &c);
  int colorTemp = sensor.calculateColorTemperature(r, g, b);
  int lux = sensor.calculateLux(r, g, b);
  Serial.print("  Red: ");
  Serial.print(r);
  Serial.print("  Green: ");
  Serial.print(g);
  Serial.print("  Blue: ");
  Serial.print(b);
  Serial.print("  Clear: ");
  Serial.println(c);
  Serial.print("  Color temperature : ");
  Serial.print(colorTemp);
  Serial.print("  Lux: ");
  Serial.println(lux);
  Serial.println(" ");
  
  return colorTemp;
}
