#include <Smartcar_sensors.h>

Sonar front, back; //initialize two ultra sound sensors
const int FRONT_TRIG_PIN = 38; //sensor's trig pin
const int FRONT_ECHO_PIN = 39; //sensor's echo pin
const int BACK_TRIG_PIN = 40;
const int BACK_ECHO_PIN = 41;

void setup() {
  front.attach(FRONT_TRIG_PIN, FRONT_ECHO_PIN); //initialize the sensor with attach(trigger pin, echo pin)
  back.attach(BACK_TRIG_PIN, BACK_ECHO_PIN);
  Serial.begin(9600); //start the serial
}

void loop() {
  delay(100); //run the following every 100ms
  Serial.print("Front: ");
  Serial.print(front.getDistance()); //print the distance in centimeters
  Serial.print(" Back: ");
  Serial.println(back.getDistance());
}
