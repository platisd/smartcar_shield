#include <Wire.h>
#include <Smartcar.h>

Odometer encoderLeft, encoderRight;
const int encoderLeftPin = 2;
const int encoderRightPin = 3;

void setup() {
  Serial.begin(9600);
  encoderLeft.attach(encoderLeftPin);
  encoderRight.attach(encoderRightPin);
  encoderLeft.begin();
  encoderRight.begin();
}

void loop() {
  Serial.print(encoderLeft.getDistance());
  Serial.print("\t\t");
  Serial.println(encoderRight.getDistance());
}


