/**
 Print out the current speed as detected by the odometers, as well as the set
 point of the pid controller, so to observe its behavior
*/

#include <Smartcar.h>

const unsigned long PRINTOUT_INTERVAL = 100;
unsigned long previousPrintOut        = 0;
float carSpeed                        = 0.5;

BrushedMotor leftMotor(smartcarlib::hw::v2::leftMotorPins);
BrushedMotor rightMotor(smartcarlib::hw::v2::rightMotorPins);
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
    Serial.begin(9600);

    car.enableCruiseControl(2, 0, 4);
    car.setSpeed(carSpeed);
}

void loop()
{
    car.update();
    if (millis() > previousPrintOut + PRINTOUT_INTERVAL)
    {
        Serial.print(carSpeed); // print the controllers set point (the speed set to the car, i.e.
                                // during setup())
        Serial.print(","); // print a comma, in order to be easily parsed by the Serial Plotter or
                           // other program
        Serial.println(car.getSpeed()); // get the average speed of the two odometers
        previousPrintOut = millis();    // update the previous print out moment
    }
}
