#include <Smartcar.h>

const int GYROSCOPE_OFFSET         = 37;
const unsigned long PRINT_INTERVAL = 100;
unsigned long previousPrintout     = 0;

BrushedMotor leftMotor(smartcarlib::hw::v2::leftMotorPins);
BrushedMotor rightMotor(smartcarlib::hw::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

GY50 gyroscope(GYROSCOPE_OFFSET);

HeadingCar car(control, gyroscope);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // Update the heading readings
    car.update();
    unsigned long currentTime = millis();
    if (currentTime >= previousPrintout + PRINT_INTERVAL)
    {
        previousPrintout = currentTime;
        Serial.println(car.getHeading());
        {
            previousPrintout = currentTime;
            Serial.println(car.getHeading());
        }
    }
