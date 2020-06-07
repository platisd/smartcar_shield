#include <Smartcar.h>

ArduinoRuntime arduinoRuntime;
GY50 gyro(arduinoRuntime, 0); // Provide the gyroscope with a random offset

void setup()
{
    Serial.begin(9600);
    Serial.println("Calibrating gyroscope, this might take some seconds");
    int offset = gyro.getOffset();
    Serial.print("This gyro's offset value is: ");
    Serial.println(offset);
    Serial.print("Please initialize Gyroscope with the above value as: GY50 gyro(");
    Serial.print(offset);
    Serial.println(
        "); or another similar value that works better according to your experimentation.");
}

void loop() {}
