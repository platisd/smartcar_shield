#include <Smartcar.h>

const int SIDE_FRONT_PIN = A0;
GP2Y0A02 sideFrontIR(SIDE_FRONT_PIN); //measure distances between 25 and 120 centimeters

void setup() {
  Serial.begin(9600); //start the serial
}

void loop() {
  Serial.println(sideFrontIR.getDistance());
  delay(100);
}
