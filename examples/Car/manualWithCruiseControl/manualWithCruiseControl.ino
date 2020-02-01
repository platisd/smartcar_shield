#include <Smartcar.h>

const float fSpeed = 0.5;  // a ground speed (m/sec) for going forward
const float bSpeed = -0.5; // a ground speed (m/sec)y for going backward
const int lDegrees = -75;  // degrees to turn left
const int rDegrees = 75;   // degrees to turn right

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

const auto odometerLeftPin       = 2;
const auto odometerRightPin      = 3;
const auto pulsesPerMeterLeft    = 50;
const auto pulsesPerMeterRight   = 60;
const unsigned short odometerPin = 2;

#ifdef ESP_BOARD
DirectionlessOdometer leftOdometer(odometerLeftPin,
                                   std::bind(&DirectionlessOdometer::update, &leftOdometer),
                                   pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(odometerRightPin,
                                    std::bind(&DirectionlessOdometer::update, &rightOdometer),
                                    pulsesPerMeterRight);
#else
DirectionlessOdometer leftOdometer(
    odometerLeftPin, []() { leftOdometer.update(); }, pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(
    odometerRightPin, []() { rightOdometer.update(); }, pulsesPerMeterRight);
#endif

DistanceCar car(control, leftOdometer, rightOdometer);

void setup()
{
    Serial.begin(9600);

    car.enableCruiseControl(); // using default PID values
}

void loop()
{
    car.update();
    handleInput();
}

void handleInput()
{ // handle serial input if there is any
    if (Serial.available())
    {
        char input = Serial.read(); // read everything that has been received so far and log down
                                    // the last entry
        switch (input)
        {
        case 'l': // rotate counter-clockwise going forward
            car.setSpeed(fSpeed);
            car.setAngle(lDegrees);
            break;
        case 'r': // turn clock-wise
            car.setSpeed(fSpeed);
            car.setAngle(rDegrees);
            break;
        case 'f': // go ahead
            car.setSpeed(fSpeed);
            car.setAngle(0);
            break;
        case 'b': // go back
            car.setSpeed(bSpeed);
            car.setAngle(0);
            break;
        default: // if you receive something that you don't know, just stop
            car.setSpeed(0);
            car.setAngle(0);
        }
    }
}
