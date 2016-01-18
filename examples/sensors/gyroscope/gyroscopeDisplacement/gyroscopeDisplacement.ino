#include <Wire.h>
#include <Smartcar.h>
#include <Servo.h>

Gyroscope gyro;

void setup() {
  gyro.attach();
  Serial.begin(9600);
  delay(1500);
  gyro.begin(90);
}

void loop() {
  gyro.update();
  Serial.println(gyro.getAngularDisplacement());
}
