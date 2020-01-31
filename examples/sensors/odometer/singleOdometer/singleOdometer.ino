#include <Smartcar.h>

DirectionlessOdometer odometer(80); // 80 pulses per meter
const int odometerPin = 2;

void setup()
{
    Serial.begin(9600);
#ifdef ESP_BOARD
    odometer.attach(odometerPin, std::bind(&DirectionlessOdometer::update, &odometer));
#else
    odometer.attach(odometerPin, []() { odometer.update(); });
#endif
}

void loop()
{
    Serial.println(odometer.getDistance());
    delay(100);
}
