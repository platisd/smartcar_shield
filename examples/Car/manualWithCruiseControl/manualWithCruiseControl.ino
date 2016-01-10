#include <Smartcar.h>
#include <Wire.h>

Odometer encoderLeft, encoderRight;
Car car;

const float fSpeed = 0.5; //a ground speed (m/sec) for going forward
const float bSpeed = -0.5; //a ground speed (m/sec)y for going backward
const int lDegrees = -75; //degrees to turn left
const int rDegrees = 75; //degrees to turn right
const int encoderLeftPin = 3;
const int encoderRightPin = 2;

void setup() {
  Serial3.begin(9600);
  encoderLeft.attach(encoderLeftPin);
  encoderRight.attach(encoderRightPin);
  encoderLeft.begin();
  encoderRight.begin();
  car.begin(encoderLeft, encoderRight); //initialize the car using the encoders
  car.enableCruiseControl(); //using default PID values
 // car.enableCruiseControl(12,5,17,30); //using custom values and control frequency
}

void loop() {
  car.updateMotors();
  handleInput();
}

void handleInput() { //handle serial input if there is any
  if (Serial3.available()) {
    char input;
    while (Serial3.available()) input = Serial3.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      case 'l': //rotate counter-clockwise going forward
        car.setSpeed(fSpeed);
        car.setAngle(lDegrees);
        break;
      case 'r': //turn clock-wise
        car.setSpeed(fSpeed);
        car.setAngle(rDegrees);
        break;
      case 'f': //go ahead
        car.setSpeed(fSpeed);
        car.setAngle(0);
        break;
      case 'b': //go back
        car.setSpeed(bSpeed);
        car.setAngle(0);
        break;
      default: //if you receive something that you don't know, just stop
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
}
