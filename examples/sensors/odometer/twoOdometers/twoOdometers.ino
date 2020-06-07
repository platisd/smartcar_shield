#include <Smartcar.h>

const auto pulsesPerMeter = 600;

ArduinoRuntime arduinoRuntime;
DirectionlessOdometer leftOdometer(
    arduinoRuntime,
    smartcarlib::pins::v2::leftOdometerPin,
    []() { leftOdometer.update(); },
    pulsesPerMeter);
DirectionlessOdometer rightOdometer(
    arduinoRuntime,
    smartcarlib::pins::v2::rightOdometerPin,
    []() { rightOdometer.update(); },
    pulsesPerMeter);

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
