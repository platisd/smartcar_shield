#include <Smartcar.h>

ArduinoRuntime arduino;
// Determine the offset for your gyroscope using the `getOffset` method
const int GYROSCOPE_OFFSET = 37;
GY50 gyro(arduino, GYROSCOPE_OFFSET);

void setup()
{
    Serial.begin(9600);
    delay(1500);
}

void loop()
{
    // Update the readings of the gyroscope
    // You should have this method being freely executed within your main loop
    gyro.update();
    Serial.println(gyro.getHeading());
}
