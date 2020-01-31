/**
 Print out the current speed as detected by the odometers, as well as the set
 point of the pid controller, so to observe its behavior
*/

#include <Smartcar.h>

const unsigned long PRINTOUT_INTERVAL   = 100;
const unsigned short LEFT_ODOMETER_PIN  = 2;
const unsigned short RIGHT_ODOMETER_PIN = 3;
unsigned long previousPrintOut          = 0;
float carSpeed                          = 0.5;

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

DirectionlessOdometer leftOdometer(78);
DirectionlessOdometer rightOdometer(78);

DistanceCar car(control, leftOdometer, rightOdometer);

void setup()
{
    Serial.begin(9600);

#ifdef ESP_BOARD
    leftOdometer.attach(LEFT_ODOMETER_PIN, std::bind(&DirectionlessOdometer::update, &leftOdometer));
    rightOdometer.attach(RIGHT_ODOMETER_PIN,
                         std::bind(&DirectionlessOdometer::update, &rightOdometer));
#else
    leftOdometer.attach(LEFT_ODOMETER_PIN, []() { leftOdometer.update(); });
    rightOdometer.attach(RIGHT_ODOMETER_PIN, []() { rightOdometer.update(); });
#endif

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
