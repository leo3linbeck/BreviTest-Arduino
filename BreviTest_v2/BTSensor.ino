void sensor_setup() {
  Serial.println(F("Setting up sensors..."));
  pinMode(pinAssaySensorVin, OUTPUT);
  pinMode(pinControlSensorVin, OUTPUT);
  pinMode(pinAssaySensorLED, OUTPUT);
  pinMode(pinControlSensorLED, OUTPUT);

//  digitalWrite(pinAssaySensorVin, HIGH);
//  digitalWrite(pinAssaySensorLED, HIGH);
//  assay_sensor.begin();
  digitalWrite(pinAssaySensorLED, LOW);
  digitalWrite(pinAssaySensorVin, LOW);
//  assay_sensor.disable();
//  delay(1000);
 
//  digitalWrite(pinControlSensorVin, HIGH);
//  digitalWrite(pinControlSensorLED, HIGH);
//  control_sensor.begin();
  digitalWrite(pinControlSensorLED, LOW);
  digitalWrite(pinControlSensorVin, LOW);
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
  digitalWrite(pinAssaySensorVin, HIGH);
  analogWrite(pinAssaySensorLED, 500);
  Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_16X);
  delay(500);
  sensor.begin();
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
  digitalWrite(pinAssaySensorVin, LOW);

//  Serial.println("Reading control sensor: ");
//  get_one_sensor_reading(pinControlSensorVin, pinControlSensorLED);
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
