#include <Smartcar.h>

const float fSpeed = 0.5;  // a ground speed (m/sec) for going forward
const float bSpeed = -0.5; // a ground speed (m/sec)y for going backward
const int lDegrees = -75;  // degrees to turn left
const int rDegrees = 75;   // degrees to turn right

ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor(arduinoRuntime, smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(arduinoRuntime, smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

const auto pulsesPerMeter = 600;

DirectionlessOdometer leftOdometer(
    arduinoRuntime,
    smartcarlib::pins::v2::leftOdometerPin,
    []() { leftOdometer.update(); },
    pulsesPerMeter);
DirectionlessOdometer rightOdometer(
    arduinoRuntime,
    smartcarlib::pins::v2::rightOdometerPin,
    []() { rightOdometer.update(); },
    pulsesPerMeter);

DistanceCar car(arduinoRuntime, control, leftOdometer, rightOdometer);

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
