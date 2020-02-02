#include <Smartcar.h>

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

const auto odometerLeftPin       = 2;
const auto odometerRightPin      = 3;
const auto pulsesPerMeterLeft    = 50;
const auto pulsesPerMeterRight   = 60;
const unsigned short odometerPin = 2;

DirectionlessOdometer leftOdometer(
    odometerLeftPin, []() { leftOdometer.update(); }, pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(
    odometerRightPin, []() { rightOdometer.update(); }, pulsesPerMeterRight);

DistanceCar car(control, leftOdometer, rightOdometer);

void setup()
{
    car.enableCruiseControl();
    car.setSpeed(1.5); // Maintain a speed of 1.5 m/sec
}

void loop()
{
    car.update(); // Maintain the speed
    // Stop after moving 1 meter
    if (car.getDistance() >= 100)
    {
        car.setSpeed(0);
    }
}
