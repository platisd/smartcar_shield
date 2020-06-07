#include <Smartcar.h>

const int SIDE_FRONT_PIN = A0;

ArduinoRuntime arduinoRuntime;
GP2Y0A21 sideFrontIR(arduinoRuntime,
                     SIDE_FRONT_PIN); // measure distances between 12 and 78 centimeters

void setup()
{
    Serial.begin(9600); // start the serial
}

void loop()
{
    Serial.println(sideFrontIR.getDistance());
    delay(100);
}
