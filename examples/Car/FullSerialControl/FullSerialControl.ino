#include <Smartcar.h>

BrushedMotor leftMotor(smartcarlib::hw::v2::leftMotorPins);
BrushedMotor rightMotor(smartcarlib::hw::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

SimpleCar car(control);

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(200);
}

void loop()
{
    handleInput();
}

void handleInput()
{
    // handle serial input if there is any
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        if (input.startsWith("m"))
        {
            int throttle = input.substring(1).toInt();
            car.setSpeed(throttle);
        }
        else if (input.startsWith("t"))
        {
            int deg = input.substring(1).toInt();
            car.setAngle(deg);
        }
    }
}
