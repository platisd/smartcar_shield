#include <Smartcar.h>

const int SERVO_PIN = 8;
const int ESC_PIN   = 9;

ServoMotor steering(SERVO_PIN);
ServoMotor throttling(ESC_PIN);
AckermanControl control(steering, throttling);

SimpleCar car(control);

void handleInput()
{ // Handle serial input if there is any
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

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(100);
}

void loop()
{
    handleInput();
}
