#include <Smartcar.h>

const int carSpeed         = 80; // 80% of the max speed
const int GYROSCOPE_OFFSET = 37;

BrushedMotor leftMotor(smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

GY50 gyroscope(GYROSCOPE_OFFSET);

HeadingCar car(control, gyroscope);

void setup()
{
    delay(1000);
    rotateOnSpot(90, carSpeed); // rotate clockwise 90 degrees on spot
    delay(1000);
    rotateOnSpot(-90, carSpeed); // rotate counter clockwise 90 degrees on spot
}

void loop() {}

/**
   Rotate the car on spot at the specified degrees with the certain speed
   @param degrees   The degrees to rotate on spot. Positive values for clockwise
                    negative for counter-clockwise.
   @param speed     The speed to rotate
*/
void rotateOnSpot(int targetDegrees, int speed)
{
    speed = smartcarlib::utils::getAbsolute(speed);
    targetDegrees %= 360; // put it on a (-360,360) scale
    if (!targetDegrees)
        return; // if the target degrees is 0, don't bother doing anything
    /* Let's set opposite speed on each side of the car, so it rotates on spot */
    if (targetDegrees > 0)
    { // positive value means we should rotate clockwise
        car.overrideMotorSpeed(speed,
                               -speed); // left motors spin forward, right motors spin backward
    }
    else
    { // rotate counter clockwise
        car.overrideMotorSpeed(-speed,
                               speed); // left motors spin backward, right motors spin forward
    }
    const auto initialHeading = car.getHeading(); // the initial heading we'll use as offset to
                                                  // calculate the absolute displacement
    int degreesTurnedSoFar
        = 0; // this variable will hold the absolute displacement from the beginning of the rotation
    while (abs(degreesTurnedSoFar) < abs(targetDegrees))
    { // while absolute displacement hasn't reached the (absolute) target, keep turning
        car.update(); // update to integrate the latest heading sensor readings
        auto currentHeading = car.getHeading(); // in the scale of 0 to 360
        if ((targetDegrees < 0) && (currentHeading > initialHeading))
        { // if we are turning left and the current heading is larger than the
            // initial one (e.g. started at 10 degrees and now we are at 350), we need to substract
            // 360, so to eventually get a signed
            currentHeading -= 360; // displacement from the initial heading (-20)
        }
        else if ((targetDegrees > 0) && (currentHeading < initialHeading))
        { // if we are turning right and the heading is smaller than the
            // initial one (e.g. started at 350 degrees and now we are at 20), so to get a signed
            // displacement (+30)
            currentHeading += 360;
        }
        degreesTurnedSoFar
            = initialHeading - currentHeading; // degrees turned so far is initial heading minus
                                               // current (initial heading
        // is at least 0 and at most 360. To handle the "edge" cases we substracted or added 360 to
        // currentHeading)
    }
    car.setSpeed(0); // we have reached the target, so stop the car
}
