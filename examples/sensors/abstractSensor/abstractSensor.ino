#include <Wire.h>
#include <Smartcar.h>

SR04 front;
const int TRIGGER_PIN = 6; //D6
const int ECHO_PIN = 7; //D7

void setup() {
  Serial.begin(9600);
  front.attach(TRIGGER_PIN, ECHO_PIN); //trigger pin, echo pin
}

void loop() {
  Serial.println(getSensorDistance(front));
  delay(100);
}

//get the distance from whichever kind of ultrasonic or infrared sensor you attach
unsigned int getSensorDistance(DistanceSensor &sensor) {
  return sensor.getDistance();
}

