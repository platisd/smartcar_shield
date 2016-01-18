#include <Wire.h>
#include <Smartcar.h>
#include <Servo.h>

SR04 front;
const int TRIGGER_PIN = 6; //D6
const int ECHO_PIN = 7; //D7

void setup() {
  Serial.begin(9600);
  front.attach(TRIGGER_PIN, ECHO_PIN); //trigger pin, echo pin
}

void loop() {
  Serial.println(front.getDistance());
  delay(100);
}

