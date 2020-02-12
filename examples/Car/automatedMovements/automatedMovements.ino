#include <Smartcar.h>

const float carSpeed        = 1.0;
const long distanceToTravel = 40;
const int degreesToTurn     = 90;

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

GY50 gyroscope(37);

const auto odometerLeftPin     = 2;
const auto odometerRightPin    = 3;
const auto pulsesPerMeterLeft  = 50;
const auto pulsesPerMeterRight = 60;

DirectionlessOdometer leftOdometer(
    odometerLeftPin, []() { leftOdometer.update(); }, pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(
    odometerRightPin, []() { rightOdometer.update(); }, pulsesPerMeterRight);

SmartCar car(control, gyroscope, leftOdometer, rightOdometer);

void setup()
{
    car.enableCruiseControl();

    // Travel around an imaginary square
    go(distanceToTravel, carSpeed);
    rotate(degreesToTurn, carSpeed);

    go(distanceToTravel, carSpeed);
    rotate(degreesToTurn, carSpeed);

    go(distanceToTravel, carSpeed);
    rotate(degreesToTurn, carSpeed);

    go(distanceToTravel, carSpeed);
    rotate(degreesToTurn, carSpeed);
}

void loop()
{
    // put your main code here, to run repeatedly:
}

/**
   Rotate the car at the specified degrees with the certain speed
   @param degrees   The degrees to turn. Positive values for clockwise
                    negative for counter-clockwise.
   @param speed     The speed to turn
*/
void rotate(int degrees, float speed)
{
    speed = smartcarlib::utils::getAbsolute(speed);
    degrees %= 360; // Put degrees in a (-360,360) scale
    if (degrees == 0)
    {
        return;
    }

    car.setSpeed(speed);
    if (degrees > 0)
    {
        car.setAngle(90);
    }
    else
    {
        car.setAngle(-90);
    }

    const auto initialHeading    = car.getHeading();
    bool hasReachedTargetDegrees = false;
    while (!hasReachedTargetDegrees)
    {
        car.update();
        auto currentHeading = car.getHeading();
        if (degrees < 0 && currentHeading > initialHeading)
        {
            // If we are turning left and the current heading is larger than the
            // initial one (e.g. started at 10 degrees and now we are at 350), we need to substract
            // 360 so to eventually get a signed displacement from the initial heading (-20)
            currentHeading -= 360;
        }
        else if (degrees > 0 && currentHeading < initialHeading)
        {
            // If we are turning right and the heading is smaller than the
            // initial one (e.g. started at 350 degrees and now we are at 20), so to get a signed
            // displacement (+30)
            currentHeading += 360;
        }
        // Degrees turned so far is initial heading minus current (initial heading
        // is at least 0 and at most 360. To handle the "edge" cases we substracted or added 360 to
        // currentHeading)
        int degreesTurnedSoFar  = initialHeading - currentHeading;
        hasReachedTargetDegrees = smartcarlib::utils::getAbsolute(degreesTurnedSoFar)
                                  >= smartcarlib::utils::getAbsolute(degrees);
    }

    car.setSpeed(0);
}

/**
   Makes the car travel at the specified distance with a certain speed
   @param centimeters   How far to travel in centimeters, positive for
                        forward and negative values for backward
   @param speed         The speed to travel
*/
void go(long centimeters, float speed)
{
    if (centimeters == 0)
    {
        return;
    }
    // Ensure the speed is towards the correct direction
    speed = smartcarlib::utils::getAbsolute(speed) * ((centimeters < 0) ? -1 : 1);
    car.setAngle(0);
    car.setSpeed(speed);

    long initialDistance          = car.getDistance();
    bool hasReachedTargetDistance = false;
    while (!hasReachedTargetDistance)
    {
        car.update();
        auto currentDistance   = car.getDistance();
        auto travelledDistance = initialDistance > currentDistance
                                     ? initialDistance - currentDistance
                                     : currentDistance - initialDistance;
        hasReachedTargetDistance
            = travelledDistance >= smartcarlib::utils::getAbsolute(centimeters);
    }
    car.setSpeed(0);
}
