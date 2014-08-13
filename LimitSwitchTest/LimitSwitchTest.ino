#define pinFrontLimitSwitch 12
#define pinBackLimitSwitch 11

void setup() {
  int initialState;
  
  Serial.begin(9600);
  pinMode(pinBackLimitSwitch, INPUT); 
  initialState = digitalRead(pinBackLimitSwitch);
  Serial.print("Setup complete. Pin = ");
  Serial.print(pinBackLimitSwitch);
  Serial.print(", state = ");
  Serial.println(initialState);
}

void loop() {
  int pinState = digitalRead(pinFrontLimitSwitch);
  if (pinState == HIGH) {
    Serial.println("Front limit switch tripped.");
  }
  pinState = digitalRead(pinBackLimitSwitch);
  if (pinState == HIGH) {
    Serial.println("Back limit switch tripped.");
  }
  delay(10); 
}
