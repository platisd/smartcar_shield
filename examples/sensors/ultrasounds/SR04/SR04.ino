#include <Smartcar.h>

const int TRIGGER_PIN           = 6; // D6
const int ECHO_PIN              = 7; // D7
const unsigned int MAX_DISTANCE = 100;

ArduinoRuntime arduino;
SR04 front(arduino, TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println(front.getDistance());
    delay(100);
}
