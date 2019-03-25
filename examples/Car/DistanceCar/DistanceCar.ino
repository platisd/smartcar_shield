#include <Smartcar.h>

const unsigned short LEFT_ODOMETER_PIN = 2;
const unsigned short RIGHT_ODOMETER_PIN = 3;

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

DirectionlessOdometer leftOdometer(100);
DirectionlessOdometer rightOdometer(100);

DistanceCar car(control, leftOdometer, rightOdometer);

void setup() {
  // Initialize the odometers (they won't work otherwise)
  leftOdometer.attach(LEFT_ODOMETER_PIN, []() {
    leftOdometer.update();
  });
  rightOdometer.attach(RIGHT_ODOMETER_PIN, []() {
    rightOdometer.update();
  });

  car.enableCruiseControl();
  car.setSpeed(1.5); // Maintain a speed of 1.5 m/sec
}

void loop() {
  car.update(); // Maintain the speed
  // Stop after moving 1 meter
  if (car.getDistance() >= 100) {
    car.setSpeed(0);
  }
}
