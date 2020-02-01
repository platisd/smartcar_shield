#include <Smartcar.h>

const unsigned short odometerPin   = 2;
const unsigned long pulsesPerMeter = 100;
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
    // Manually roll the vehicle for one meter
    // and note down the printed out value. That is the
    // correct pulses:centimeters ratio for your odometer
    // that you should use when initializing the odometer
    // in its constructor
    Serial.println(odometer.getDistance());
    delay(100);
}
