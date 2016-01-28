#include <Smartcar.h>

Gyroscope gyro;
Car car;
int motorSpeed = 80; //80% of the max speed

void setup() {
  gyro.attach();
  car.begin(gyro);
  rotateOnSpot(90); //rotate clockwise 90 degrees on spot
  delay(1000);
  rotateOnSpot(-90); //rotate counter clockwise 90 degrees on spot
}

void loop() {
}

void rotateOnSpot(int targetDegrees){
  if (!targetDegrees) return; //if the target degrees is 0, don't bother doing anything
  gyro.begin(); //initiate the measurement
  /* Let's set opposite speed on each side of the car, so it rotates on spot */
  if (targetDegrees > 0){ //positive value means we should rotate clockwise
    car.setMotorSpeed(motorSpeed, -motorSpeed); // left motors spin forward, right motors spin backward
  }else{ //rotate counter clockwise
    car.setMotorSpeed(-motorSpeed, motorSpeed); // left motors spin backward, right motors spin forward
  }
  while (abs(gyro.getAngularDisplacement()) <= abs(targetDegrees)){ //while we have not reached the desired degrees
    gyro.update(); //update the gyro readings
  }
  car.stop(); //we have reached the target, so stop the car
}

