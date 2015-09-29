#include <Wire.h>
#include <Servo.h>
#include <CaroloCup.h>

Odometer encoder, encoder1, encoder2;
void setup() {
  Serial.begin(9600);
  encoder.attach(2);
  encoder.begin();
}

void loop() {
  Serial.println(encoder.getDistance());
  delay(100);
}
