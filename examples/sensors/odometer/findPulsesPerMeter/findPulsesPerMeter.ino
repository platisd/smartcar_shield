#include <Smartcar.h>

// Initialize the odometer with a 1:1 pulses per cm ratio
DirectionlessOdometer odometer(100);
const int odometerPin = 2;

void setup() {
  Serial.begin(9600);
  odometer.attach(odometerPin, []() {
    odometer.update();
  });
}

void loop() {
  // Manually roll the vehicle for one meter
  // and note down the printed out value. That is the
  // correct pulses:centimeters ratio for your odometer
  // that you should use when initializing the odometer
  // in its constructor
  Serial.print(odometer.getDistance());
  delay(100);
}


