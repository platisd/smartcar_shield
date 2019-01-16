#include <Smartcar.h>

DirectionlessOdometer leftOdometer(50), rightOdometer(60);
const int odometerLeftPin = 2;
const int odometerRightPin = 3;

void setup() {
  Serial.begin(9600);
  leftOdometer.attach(odometerLeftPin, []() {
    leftOdometer.update();
  });
  rightOdometer.attach(odometerRightPin, []() {
    rightOdometer.update();
  });
}

void loop() {
  Serial.print(leftOdometer.getDistance());
  Serial.print("\t\t");
  Serial.println(rightOdometer.getDistance());
}


