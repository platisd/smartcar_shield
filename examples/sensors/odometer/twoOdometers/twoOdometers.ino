#include <Smartcar.h>

DirectionlessOdometer leftOdometer(50), rightOdometer(60);
const int odometerLeftPin  = 2;
const int odometerRightPin = 3;

void setup()
{
    Serial.begin(9600);
#ifdef ESP_BOARD
    leftOdometer.attach(LEFT_ODOMETER_PIN, std::bind(&DirectionlessOdometer::update, &leftOdometer));
    rightOdometer.attach(RIGHT_ODOMETER_PIN,
                         std::bind(&DirectionlessOdometer::update, &rightOdometer));
#else
    leftOdometer.attach(LEFT_ODOMETER_PIN, []() { leftOdometer.update(); });
    rightOdometer.attach(RIGHT_ODOMETER_PIN, []() { rightOdometer.update(); });
#endif
}

void loop()
{
    Serial.print(leftOdometer.getDistance());
    Serial.print("\t\t");
    Serial.println(rightOdometer.getDistance());
}
