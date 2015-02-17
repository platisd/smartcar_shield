#include <Smartcar_sensors.h>
#include <Wire.h>

Gyroscope gyro;

void setup() {
  Serial.begin(9600);
  gyro.attach();
  delay(1500);
  Serial.println("Start measuring!");
  gyro.begin();
}

void loop() {
  Serial.println(gyro.getAngularDisplacement());
  delay(100);
}

