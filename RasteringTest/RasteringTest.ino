#define pinMotorEnableA 9
#define pinMotorEnableB 8
#define pinMotorDirectionA 12
#define pinMotorDirectionB 13
#define pinMotorPowerA 3
#define pinMotorPowerB 11

#define pinSolenoid 44
#define pinFrontLimitSwitch 24
#define pinBackLimitSwitch 22

#define rasterButton 2
#define forwardButton 4
#define backwardButton 6

#define mmPerRotation 1.0
#define secsPerSolenoidShift 0.5
#define mmPerRaster 0.3
#define stepsPerRotation 200.0
#define rpm 100.0

double rps = rpm / 60.0;
double mmPerSec = mmPerRotation * rps;
double stepsPerSec = stepsPerRotation * rps;
double delayAtFullSpeed = 1000.0 / (stepsPerRotation * rps);
double mmPerStep = mmPerRotation / stepsPerRotation;
int stepsPerRaster = int(round(mmPerRaster / mmPerStep));

int rasterButtonState;
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
  pinMode(rasterButton, INPUT);
  pinMode(forwardButton, INPUT);
  pinMode(backwardButton, INPUT);

  rasterButtonState = digitalRead(rasterButton);
  forwardButtonState = digitalRead(forwardButton);
  backwardButtonState = digitalRead(backwardButton);

  pinMode(pinSolenoid, OUTPUT);
  analogWrite(pinSolenoid, 0);
  
  resetXStage();
}

void resetXStage() {
  int i = 0;
  while(digitalRead(pinBackLimitSwitch) == LOW) {
    moveBackwardOneStep(i % 4, delayAtFullSpeed);
    i += 1;
  }
  powerOffStepper();
}

void loop() {
  int rVal, rVal2, fVal, fVal2, bVal, bVal2;
  
  rVal = digitalRead(rasterButton);
  fVal = digitalRead(forwardButton);
  bVal = digitalRead(backwardButton);
  delay(DEBOUNCE);
  rVal2 = digitalRead(rasterButton);
  fVal2 = digitalRead(forwardButton);
  bVal2 = digitalRead(backwardButton);
  
  if (rVal == rVal2) {
    if (rVal != rasterButtonState) {
      if (rVal == HIGH) {
        rasterAssayWell(6.0);
      }
      else {
        rasterButtonState = rVal;
      }
    }
  }
  
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

void solenoidOut() {
    Serial.println("Shifting out...");
    analogWrite(pinSolenoid, 0);
}

void solenoidIn() {
    Serial.println("Shifting in...");
    analogWrite(pinSolenoid, 255);
}

void rasterAssayWell(double mm) {
  int steps = int(round(mm / mmPerStep));
  
  Serial.print("Rastering assay well, length = ");
  Serial.print(mm);
  Serial.print("mm, steps = ");
  Serial.println(steps);
  for (int i = 0; i < steps; i += 1) {
    moveForwardOneStep(i % 4, delayAtFullSpeed);
    if ((i % stepsPerRaster) == 0) {
      if (digitalRead(pinSolenoid) == LOW) {
        solenoidIn();
      }
      else {
        solenoidOut();
      }
      delay(int(round(secsPerSolenoidShift * 1000.0)));
    }
  }
  solenoidOut();
  powerOffStepper();
}

void moveForwardOneRotation() {
  Serial.println("Forward one rotation...");
  moveForwardLength(1.0);
  powerOffStepper();
}

void moveBackwardOneRotation() {
  Serial.println("Backward one rotation...");
  moveBackwardLength(1.0);
  powerOffStepper();
}

void moveForwardLength(double mm) {
  int steps = int(round(mm / mmPerStep));
  for (int i = 0; i < steps; i += 1) {
    if (digitalRead(pinFrontLimitSwitch) == LOW) {
      moveForwardOneStep(i % 4, delayAtFullSpeed);
    }
  }
  forwardButtonState = LOW;
}

void moveBackwardLength(double mm) {
  int steps = int(round(mm / mmPerStep));
  for (int i = 0; i < steps; i += 1) {
    if (digitalRead(pinBackLimitSwitch) == LOW) {
      moveBackwardOneStep(i % 4, delayAtFullSpeed);
    }
  }
  backwardButtonState = LOW;
}

void moveForwardOneStep(int mode, int delayLength) {
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

void moveBackwardOneStep(int mode, int delayLength) {
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

void powerOffStepper() {
    digitalWrite(pinMotorPowerA, LOW);   //Moves CH B
    digitalWrite(pinMotorPowerB, LOW);   //Moves CH B
}
