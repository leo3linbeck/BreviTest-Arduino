#include <EEPROM.h>

char serial_number[20] = "OBAD-BWUG-NCPA-WIAF";
char old_SN[20];
char verify[20];

void setup() {
  int i;
  
  Serial.begin(9600);
  delay(2000);
  
  if (EEPROM.read(0) == 0xFF) {
    Serial.println("No serial number has been written yet");
  }
  else {
    for (i = 0; i < 19; i += 1) {
      old_SN[i] = EEPROM.read(i);
    }
  
    Serial.print("Existing serial number: ");
    Serial.println(old_SN);
  }

  Serial.print("Writing new serial number: ");
  Serial.println(serial_number);
  
  for (i = 0; i < 19; i += 1) {
    EEPROM.write(i,serial_number[i]);
  }

  Serial.println("Verifying serial number...");
  for (i = 0; i < 19; i += 1) {
    verify[i] = EEPROM.read(i);
  }
  Serial.print("Verified serial number: ");
  Serial.println(verify);

  Serial.println("Serial number writing complete.");
}

void loop() {
  while (true);
}
