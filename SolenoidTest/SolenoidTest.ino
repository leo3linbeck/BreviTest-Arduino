#define signalPin 3

void setup() {
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW);
}

void loop() {
  for (int i = 0; i < 5; i += 1) {
    delay(200);
    analogWrite(signalPin, HIGH);
    delay(200);
    digitalWrite(signalPin, LOW);
  }
  while(true);
}

