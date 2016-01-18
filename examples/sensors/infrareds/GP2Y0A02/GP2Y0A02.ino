#include <Smartcar.h>
#include <Wire.h>
#include <Servo.h>

GP2Y0A02 sideFrontIR; //measure distances between 25 and 120 centimeters
const int SIDE_FRONT_PIN = A0;

void setup() {
  sideFrontIR.attach(SIDE_FRONT_PIN);
  Serial.begin(9600); //start the serial
}

void loop() {
  Serial.println(sideFrontIR.getDistance());
  delay(100);
}
