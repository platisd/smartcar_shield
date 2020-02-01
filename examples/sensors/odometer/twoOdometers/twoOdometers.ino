#include <Smartcar.h>

const auto odometerLeftPin       = 2;
const auto odometerRightPin      = 3;
const auto pulsesPerMeterLeft    = 50;
const auto pulsesPerMeterRight   = 60;
const unsigned short odometerPin = 2;

#ifdef ESP_BOARD
DirectionlessOdometer leftOdometer(odometerLeftPin,
                                   std::bind(&DirectionlessOdometer::update, &leftOdometer),
                                   pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(odometerRightPin,
                                    std::bind(&DirectionlessOdometer::update, &rightOdometer),
                                    pulsesPerMeterRight);
#else
DirectionlessOdometer leftOdometer(
    odometerLeftPin, []() { leftOdometer.update(); }, pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(
    odometerRightPin, []() { rightOdometer.update(); }, pulsesPerMeterRight);
#endif

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.print(leftOdometer.getDistance());
    Serial.print("\t\t");
    Serial.println(rightOdometer.getDistance());
}
