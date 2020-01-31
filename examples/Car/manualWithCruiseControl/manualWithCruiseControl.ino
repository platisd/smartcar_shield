#include <Smartcar.h>

const float fSpeed         = 0.5;  // a ground speed (m/sec) for going forward
const float bSpeed         = -0.5; // a ground speed (m/sec)y for going backward
const int lDegrees         = -75;  // degrees to turn left
const int rDegrees         = 75;   // degrees to turn right
const int leftOdometerPin  = 2;
const int rightOdometerPin = 3;

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

DirectionlessOdometer leftOdometer(110);
DirectionlessOdometer rightOdometer(120);

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
