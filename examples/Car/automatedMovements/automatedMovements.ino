#include <Smartcar.h>
#include <Wire.h>

Odometer encoderLeft, encoderRight;
Gyroscope gyro;
Car car;

void setup() {
  gyro.attach();
  encoderLeft.attach(2);
  encoderRight.attach(3);
  encoderLeft.begin();
  encoderRight.begin();
  car.begin(encoderLeft, encoderRight,gyro);
  car.enableCruiseControl();
  gyro.begin();
  car.go(40);
  car.rotate(90);
  car.go(40);
  car.rotate(90);
  car.go(40);
  car.rotate(90);
  car.go(40);
  car.rotate(90);
}

void loop() {
  // put your main code here, to run repeatedly:

}
