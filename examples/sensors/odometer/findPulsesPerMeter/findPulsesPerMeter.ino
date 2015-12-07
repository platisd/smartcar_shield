#include <Wire.h>
#include <Smartcar.h>

Odometer encoder(100); //let's say there are exactly 100 pulser per meter
const int encoderPin = 2;

void setup() {
  Serial.begin(9600);
  encoder.attach(encoderPin);
  encoder.begin();
}

void loop() {
  Serial.print(encoder.getDistance()); //now we get a 1 to 1 mapping of pulses to cm
  delay(100);
}


