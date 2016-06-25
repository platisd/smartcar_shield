#include <Smartcar.h>

Gyroscope gyro; //you should provide the offset for your own gyroscope (use the gyroscopeCalibration sketch)

void setup() {
  gyro.attach();
  Serial.begin(9600);
  delay(1500);
  gyro.begin(90); //sample the readings from the gyroscope every 90 milliseconds. If no argument supplied, the default sampling period will be used.
}

void loop() {
  gyro.update(); //update the readings of the gyroscope, you should have this method being freely executed within your main loop
  Serial.println(gyro.getAngularDisplacement());
}
