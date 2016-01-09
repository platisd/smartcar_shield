#include <Smartcar.h>
#include <Wire.h>

Car car(Car::INVERTED);

const int fSpeed = 200; //a PWM frequency for going forward
const int bSpeed = -200; //a PWM frequency for going backward
const int lDegrees = -75; //degrees to turn left
const int rDegrees = 75; //degrees to turn right


void setup() {
  Serial3.begin(9600);
  car.begin(); //initialize the car using the encoders and the gyro
}

void loop() {
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
