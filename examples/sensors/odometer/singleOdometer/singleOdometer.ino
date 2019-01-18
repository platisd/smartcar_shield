#include <Smartcar.h>

DirectionlessOdometer odometer(80); // 80 pulses per meter
const int odometerPin = 2;

void setup() {
  Serial.begin(9600);
  odometer.attach(odometerPin, []() {
    odometer.update();
  });
}

void loop() {
  Serial.print(odometer.getDistance());
  delay(100);
}


