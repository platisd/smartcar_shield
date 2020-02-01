#include <Smartcar.h>

const unsigned short odometerPin   = 2;
const unsigned long pulsesPerMeter = 400;
#ifdef ESP_BOARD
DirectionlessOdometer
    odometer(odometerPin, std::bind(&DirectionlessOdometer::update, &odometer), pulsesPerMeter);
#else
DirectionlessOdometer odometer(
    odometerPin, []() { odometer.update(); }, pulsesPerMeter);
#endif

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println(odometer.getDistance());
    delay(100);
}
