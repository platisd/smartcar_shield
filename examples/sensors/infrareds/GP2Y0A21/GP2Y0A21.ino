#include <Smartcar.h>
#include <Wire.h>

GP2Y0A21 sideFrontIR; //measure distances between 12 and 78 centimeters
const int SIDE_FRONT_PIN = A0;

void setup() {
  sideFrontIR.attach(SIDE_FRONT_PIN);
  Serial.begin(9600); //start the serial
}

void loop() {
  Serial.println(sideFrontIR.getDistance());
  delay(100);
}
