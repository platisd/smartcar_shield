#include <Smartcar.h>

const unsigned short odometerPin   = 2;
const unsigned long pulsesPerMeter = 400;

DirectionlessOdometer odometer(
    odometerPin, []() { odometer.update(); }, pulsesPerMeter);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println(odometer.getDistance());
    delay(100);
}
