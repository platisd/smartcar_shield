#include <Smartcar.h>

const unsigned long PRINT_INTERVAL = 100;
unsigned long previousPrintout     = 0;

ArduinoRuntime arduino;
BrushedMotor leftMotor(arduino, smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(arduino, smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

GY50 gyroscope(arduino, 37);

const auto pulsesPerMeter = 600;

DirectionlessOdometer leftOdometer(
    arduino,
    smartcarlib::pins::v2::leftOdometerPin,
    []() { leftOdometer.update(); },
    pulsesPerMeter);
DirectionlessOdometer rightOdometer(
    arduino,
    smartcarlib::pins::v2::rightOdometerPin,
    []() { rightOdometer.update(); },
    pulsesPerMeter);

SmartCar car(arduino, control, gyroscope, leftOdometer, rightOdometer);

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
