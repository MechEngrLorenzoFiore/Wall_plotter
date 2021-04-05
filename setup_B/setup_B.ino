/*
 * This program drives a stepper for a full revolution in order to
 * measure the mm/step factor
 */

#include <Stepper.h>

const int stepsPerRevolution = 2038;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11); 
int dir = 0;

void setup() {
  myStepper.setSpeed(12);
  Serial.begin(9600);
}

void loop() {
  if (dir == 0){
  while(!Serial.available()){}
  dir = Serial.parseInt();
  Serial.flush();
  }
  switch (dir){
    case 1:
      Serial.println("clockwise");
      myStepper.step(stepsPerRevolution);
      dir = 0;
      Serial.println("Take the measurement");
      break;
    case 2:
      Serial.println("counterclockwise");
      myStepper.step(-stepsPerRevolution);
      dir = 0;
      Serial.println("Take the measurement");
      break;
  }
}
