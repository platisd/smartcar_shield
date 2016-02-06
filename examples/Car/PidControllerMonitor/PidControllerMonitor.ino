/* Print out the current speed as detected by the odometers, as well as the set point of the pid controller, so to observe its behavior */

#include <Smartcar.h>

Odometer encoderLeft(94), encoderRight(94);
Car car;
const unsigned long PRINTOUT_INTERVAL = 100;
unsigned long previousPrintOut = 0;
float carSpeed = 0.5;

void setup() {
  Serial.begin(9600);
  encoderLeft.attach(3);
  encoderRight.attach(2);
  encoderLeft.begin();
  encoderRight.begin();
  car.begin(encoderLeft, encoderRight);
  car.enableCruiseControl(2,0,4);
  car.setSpeed(carSpeed);
}

void loop() {
  car.updateMotors();
  if (millis() > previousPrintOut + PRINTOUT_INTERVAL) {
    Serial.print(car.getSpeed()); //print the controllers set point (the speed set to the car, i.e. during setup())
    Serial.print(","); //print a comma, in order to be easily parsed by the Serial Plotter or other program
    Serial.println((encoderLeft.getSpeed() + encoderRight.getSpeed()) / 2); //get the average speed of the two odometers
    previousPrintOut = millis(); //update the previous print out moment
  }

}
