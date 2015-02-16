#include <Smartcar_sensors.h>

Odometer encoder;
unsigned long startTime;
unsigned int timeOver = 20000; //20 seconds
const int encoderPin = 19; //digital pin 19 (interrupt 4 on arduino mega)

void setup() {
  Serial.begin(9600);
  encoder.attach(encoderPin);
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
