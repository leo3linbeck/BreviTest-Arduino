#include <SoftI2CMaster.h>

SoftI2CMaster i2c;

void setup() {
  i2c = SoftI2CMaster(A2, A3);
  i2c.beginTransmission(0x44);
}

void loop() {
//  i2c.write((uint8_t) 0x80 | 0x12);
  i2c.write((uint8_t) 0x0F);
  delay(1);
}
