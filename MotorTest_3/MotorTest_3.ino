#define pinMotorEnableA 9
#define pinMotorEnableB 8
#define pinMotorDirectionA 12
#define pinMotorDirectionB 13
#define pinMotorPowerA 3
#define pinMotorPowerB 11
#define forwardButton 4
#define backwardButton 2

int stepsPerRotation = 200.0;
int rpm = 90.0;
int delayLength = int (1000.0 / (stepsPerRotation * rpm / 60.0));
int forwardButtonState;
int backwardButtonState;

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

void setup() {
  Serial.begin(9600);
  //establish PWM pins
  pinMode(pinMotorPowerA, OUTPUT); //move CH A
  pinMode(pinMotorPowerB, OUTPUT); //move CH B

  //establish motor direction toggle pins
  pinMode(pinMotorDirectionA, OUTPUT); //CH A -- HIGH = forwards and LOW = backwards???
  pinMode(pinMotorDirectionB, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards???
  
  //establish motor brake pins
  pinMode(pinMotorEnableA, OUTPUT); //brake (disable) CH A
  pinMode(pinMotorEnableB, OUTPUT); //brake (disable) CH B
  
    //establish button pins
  pinMode(forwardButton, INPUT);
  pinMode(backwardButton, INPUT);

  forwardButtonState = digitalRead(forwardButton);
  backwardButtonState = digitalRead(backwardButton);
}

void loop() {
  int fVal, fVal2, bVal, bVal2;
  
  fVal = digitalRead(forwardButton);
  bVal = digitalRead(backwardButton);
  delay(DEBOUNCE);
  fVal2 = digitalRead(forwardButton);
  bVal2 = digitalRead(backwardButton);
  
  if (fVal == fVal2) {
    if (fVal != forwardButtonState) {
      if (fVal == HIGH) {
        moveForwardOneRotation();
      }
      else {
        forwardButtonState = fVal;
      }
    }
  }
  
  if (bVal == bVal2) {
    if (bVal != backwardButtonState) {
      if (bVal == HIGH) {
        moveBackwardOneRotation();
      }
      else {
        backwardButtonState = bVal;
      }
    }
  }
}

void moveForwardOneRotation() {
  Serial.println("Forward one rotation...");
  for (int i = 0; i < stepsPerRotation; i += 1) {
    moveForwardOneStep(i % 4);
  }
  forwardButtonState = LOW;
}

void moveBackwardOneRotation() {
  Serial.println("Backward one rotation...");
  for (int i = 0; i < stepsPerRotation; i += 1) {
    moveBackwardOneStep(i % 4);
  }
  backwardButtonState = LOW;
}

void moveForwardOneStep(int mode) {
  if (mode == 0) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, HIGH);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 1) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, LOW);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  else if (mode == 2) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, LOW);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 3) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, HIGH);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  delay(delayLength);
}

void moveBackwardOneStep(int mode) {
  if (mode == 0) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, HIGH);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 1) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, HIGH);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  else if (mode == 2) {
    digitalWrite(pinMotorEnableA, LOW);  //ENABLE CH A
    digitalWrite(pinMotorEnableB, HIGH); //DISABLE CH B
    digitalWrite(pinMotorDirectionA, LOW);   //Sets direction of CH A
    digitalWrite(pinMotorPowerA, HIGH);   //Moves CH A
  }
  else if (mode == 3) {
    digitalWrite(pinMotorEnableA, HIGH);  //DISABLE CH A
    digitalWrite(pinMotorEnableB, LOW); //ENABLE CH B
    digitalWrite(pinMotorDirectionB, LOW);   //Sets direction of CH B
    digitalWrite(pinMotorPowerB, HIGH);   //Moves CH B
  }
  delay(delayLength);
}
