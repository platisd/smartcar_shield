#include <Smartcar.h>

Odometer encoder;
const int encoderPin = 2;

void setup() {
  Serial.begin(9600);
  encoder.attach(encoderPin);
  encoder.begin();
}

void loop() {
  Serial.print(encoder.getDistance());
  delay(100);
}


