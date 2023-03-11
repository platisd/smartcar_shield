#include <Smartcar.h>

ArduinoRuntime arduinoRuntime;
const BrushedMotorPins leftFrontMotorPins{ 12, 14, 13 };
const BrushedMotorPins leftRearMotorPins{ 22, 24, 23 };
const BrushedMotorPins rightFrontMotorPins{ 25, 26, 27 };
const BrushedMotorPins rightRearMotorPins{ 32, 34, 33 };

BrushedMotor leftFrontMotor{ arduinoRuntime, leftFrontMotorPins };
BrushedMotor leftRearMotor{ arduinoRuntime, leftRearMotorPins };
BrushedMotor rightFrontMotor{ arduinoRuntime, rightFrontMotorPins };
BrushedMotor rightRearMotor{ arduinoRuntime, rightRearMotorPins };

DualMotor leftMotor{ leftFrontMotor, leftRearMotor };
DualMotor rightMotor{ rightFrontMotor, rightRearMotor };

DifferentialControl control{ leftMotor, rightMotor };

SimpleCar car{ control };

void setup()
{
    // Move forward for 3 seconds
    car.setSpeed(50);
    delay(3000);
    // Start turning sharp left
    car.setAngle(-90);
    delay(3000);
    // Straighten the car and move backward
    car.setAngle(0);
    car.setSpeed(-50);
    delay(3000);
    car.setSpeed(0);
}

void loop() {}
