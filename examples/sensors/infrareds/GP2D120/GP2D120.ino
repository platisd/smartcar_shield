#include <Smartcar.h>
#include <Wire.h>

GP2D120 sideFrontIR; //measure distances between 5 and 25 centimeters
const int SIDE_FRONT_PIN = A0;

void setup() {
  sideFrontIR.attach(SIDE_FRONT_PIN);
  Serial.begin(9600); //start the serial
}

void loop() {
  Serial.println(sideFrontIR.getDistance());
  delay(100);
}
