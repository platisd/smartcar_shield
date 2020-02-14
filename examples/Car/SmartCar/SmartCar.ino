#include <Smartcar.h>

const unsigned long PRINT_INTERVAL = 100;
unsigned long previousPrintout     = 0;

BrushedMotor leftMotor(smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

GY50 gyroscope(37);

const auto odometerLeftPin       = 2;
const auto odometerRightPin      = 3;
const auto pulsesPerMeterLeft    = 50;
const auto pulsesPerMeterRight   = 60;
const unsigned short odometerPin = 2;

DirectionlessOdometer leftOdometer(
    odometerLeftPin, []() { leftOdometer.update(); }, pulsesPerMeterLeft);
DirectionlessOdometer rightOdometer(
    odometerRightPin, []() { rightOdometer.update(); }, pulsesPerMeterRight);

SmartCar car(control, gyroscope, leftOdometer, rightOdometer);

void setup()
{
    Serial.begin(9600);

    car.enableCruiseControl();
    car.setSpeed(1.5); // Maintain a speed of 1.5 m/sec
}

void loop()
{
    // Maintain the speed and update the heading
    car.update();
    // Stop after moving 1 meter
    if (car.getDistance() >= 100)
    {
        car.setSpeed(0);
    }

    // Keep printing out the current heading
    unsigned long currentTime = millis();
    if (currentTime >= previousPrintout + PRINT_INTERVAL)
    {
        previousPrintout = currentTime;
        Serial.println(car.getHeading());
    }
}
