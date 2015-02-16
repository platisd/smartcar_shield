#include <Smartcar_sensors.h>

Odometer encoder;
unsigned long startTime;
unsigned int timeOver = 20000; //20 seconds

void setup() {
  Serial.begin(9600);
  encoder.attach(19);
  encoder.begin();
  startTime = millis();
}

void loop() {
  
  if (millis()-startTime>timeOver){ //count until timeOver seconds and then detach the encoder
    encoder.detach();  
  }
  delay(200);
  Serial.println(encoder.getDistance());
}
