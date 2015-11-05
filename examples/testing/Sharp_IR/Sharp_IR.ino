#include <CaroloCup.h>
#include <Servo.h>
#include <Wire.h>
Servo esc;

Sharp_IR sideFrontIR;
const int SIDE_FRONT_PIN = A0;

void setup() {
  sideFrontIR.attach(SIDE_FRONT_PIN);
  Serial.begin(9600); //start the serial
}

void loop() {
  delay(100); //run the following every 100 ms
  Serial.println(sideFrontIR.getDistance());
}
