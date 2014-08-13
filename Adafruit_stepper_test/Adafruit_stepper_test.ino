#include <AFMotor.h>

AF_Stepper motor(200, 1);

void setup() {
  Serial.begin(9600);
  
    // set up stepping motor
  Serial.println("Initializing motor and solenoid");
  motor.setSpeed(60);
}

void loop() {
  while(true){
    motor.step(10, FORWARD, SINGLE);
    delay(1000);
  };
}
