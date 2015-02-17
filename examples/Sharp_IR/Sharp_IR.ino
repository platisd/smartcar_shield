#include <Smartcar_sensors.h>
#include <Wire.h>

Sharp_IR front; //instantiate the infra red sensor
const int IR_pin = A5; //set the pin that the infra red sensor will be providing data to

void setup() {
  front.attach(A5); //attach (initialize) the sensor at the appropriate pin
  Serial.begin(9600); //start the serial
}

void loop() {
  delay(100); //run the following every 100 ms
  Serial.println(front.getMedianDistance()); //print the distance in cm

}
