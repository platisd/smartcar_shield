#include <Smartcar.h>

const int SIDE_FRONT_PIN = A0; // you can use only analog enabled pins

ArduinoRuntime arduinoRuntime;
GP2D120 sideFrontIR(arduinoRuntime,
                    SIDE_FRONT_PIN); // measure distances between 5 and 25 centimeters

void setup()
{
    Serial.begin(9600); // start the serial
}

void loop()
{
    Serial.println(sideFrontIR.getDistance());
    delay(100);
}
