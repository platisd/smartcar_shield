#include <Smartcar.h>

const unsigned long LEFT_PULSES_PER_METER  = 600;
const unsigned long RIGHT_PULSES_PER_METER = 740;

ArduinoRuntime arduinoRuntime;
DirectionalOdometer leftOdometer{ arduinoRuntime,
                                  smartcarlib::pins::v2::leftOdometerPins,
                                  []() { leftOdometer.update(); },
                                  LEFT_PULSES_PER_METER };
DirectionalOdometer rightOdometer{ arduinoRuntime,
                                   smartcarlib::pins::v2::rightOdometerPins,
                                   []() { rightOdometer.update(); },
                                   RIGHT_PULSES_PER_METER };

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
