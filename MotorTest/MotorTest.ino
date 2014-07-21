#include <Stepper.h>

#define motorSteps 200     // change this depending on the number of steps
                           // per revolution of your motor
#define motorPin1 11
#define motorPin2 3
#define motorPin3 8
#define motorPin4 9

// initialize of the Stepper library:
Stepper myStepper(motorSteps, motorPin1, motorPin2, motorPin3,motorPin4); 

void setup() {
  // set the motor speed at 60 RPMS:
  myStepper.setSpeed(60);

  // Initialize the Serial port:
  Serial.begin(9600);
}

void loop() {
  // Step forward 100 steps:
  Serial.println("Forward");
  myStepper.step(10);
  delay(500);

  // Step backward 100 steps:
  Serial.println("Backward");
  myStepper.step(-10);
  delay(500); 

}

