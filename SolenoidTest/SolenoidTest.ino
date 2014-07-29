#define signalPin 6

void setup() {
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW);
}

void loop() {
  for (int i = 0; i < 5; i += 1) {
    delay(400);
    digitalWrite(signalPin, HIGH);
    delay(400);
    digitalWrite(signalPin, LOW);
  }
  while(true);
}

