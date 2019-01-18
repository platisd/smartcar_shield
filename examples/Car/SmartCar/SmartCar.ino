#include <Smartcar.h>

const unsigned short LEFT_ODOMETER_PIN = 2;
const unsigned short RIGHT_ODOMETER_PIN = 3;
const unsigned long PRINT_INTERVAL = 100;
unsigned long previousPrintout = 0;

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

GY50 gyroscope(37);
DirectionlessOdometer leftOdometer(100);
DirectionlessOdometer rightOdometer(100);

SmartCar car(control, gyroscope, leftOdometer, rightOdometer);

void setup() {
  Serial.begin(9600);
  // Initialize the odometers (they won't work otherwise)
  leftOdometer.attach(LEFT_ODOMETER_PIN, []() {
    leftOdometer.update();
  });
  rightOdometer.attach(RIGHT_ODOMETER_PIN, []() {
    rightOdometer.update();
  });

  car.enableCruiseControl();
  car.setSpeed(1.5); // Maintain a speed of 1.5 m/sec
}

void loop() {
  // Maintain the speed and update the heading
  car.update();
  // Stop after moving 1 meter
  if (car.getDistance() >= 100) {
    car.setSpeed(0);
  }

  // Keep printing out the current heading
  unsigned long currentTime = millis();
  if (currentTime >= previousPrintout + PRINT_INTERVAL) {
    previousPrintout = currentTime;
    Serial.println(car.getHeading());
  }
}
