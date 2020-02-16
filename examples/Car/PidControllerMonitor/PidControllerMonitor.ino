/**
 Print out the current speed as detected by the odometers, as well as the set
 point of the pid controller, so to observe its behavior
*/

#include <Smartcar.h>

const unsigned long PRINTOUT_INTERVAL = 100;
unsigned long previousPrintOut        = 0;
float carSpeed                        = 0.5;

BrushedMotor leftMotor(smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

const auto pulsesPerMeter = 600;

DirectionlessOdometer leftOdometer(
    smartcarlib::pins::v2::leftOdometerPin, []() { leftOdometer.update(); }, pulsesPerMeter);
DirectionlessOdometer rightOdometer(
    smartcarlib::pins::v2::rightOdometerPin, []() { rightOdometer.update(); }, pulsesPerMeter);

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
