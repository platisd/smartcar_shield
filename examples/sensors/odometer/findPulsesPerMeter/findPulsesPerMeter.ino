#include <Smartcar.h>

const unsigned short odometerPin   = 2;
const unsigned long pulsesPerMeter = 100;

ArduinoRuntime arduinoRuntime;
DirectionlessOdometer odometer(
    arduinoRuntime, odometerPin, []() { odometer.update(); }, pulsesPerMeter);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // Manually roll the vehicle for one meter
    // and note down the printed out value. That is the
    // correct pulses:centimeters ratio for your odometer
    // that you should use when initializing the odometer
    // in its constructor
    Serial.println(odometer.getDistance());
    delay(100);
}
