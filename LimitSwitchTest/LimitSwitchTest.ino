#define pinBackLimitSwitch 22

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
  int pinState = digitalRead(pinBackLimitSwitch);
  if (pinState == HIGH) {
    Serial.println("Limit switch tripped.");
  }
  delay(10); 
}
