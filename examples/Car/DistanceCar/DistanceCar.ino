#include <Smartcar.h>

ArduinoRuntime arduinoRuntime;
BrushedMotor leftMotor(arduinoRuntime, smartcarlib::pins::v2::leftMotorPins);
BrushedMotor rightMotor(arduinoRuntime, smartcarlib::pins::v2::rightMotorPins);
DifferentialControl control(leftMotor, rightMotor);

const auto pulsesPerMeter = 600;

DirectionlessOdometer leftOdometer{ arduinoRuntime,
                                    smartcarlib::pins::v2::leftOdometerPin,
                                    []() { leftOdometer.update(); },
                                    pulsesPerMeter };
DirectionlessOdometer rightOdometer{ arduinoRuntime,
                                     smartcarlib::pins::v2::rightOdometerPin,
                                     []() { rightOdometer.update(); },
                                     pulsesPerMeter };

DistanceCar car(arduinoRuntime, control, leftOdometer, rightOdometer);

void setup()
{
    car.enableCruiseControl();
    car.setSpeed(1.5); // Maintain a speed of 1.5 m/sec
}

void loop()
{
    car.update(); // Maintain the speed
    // Stop after moving 1 meter
    if (car.getDistance() >= 100)
    {
        car.setSpeed(0);
    }
}
