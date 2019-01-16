#include <Smartcar.h>

const int TRIGGER_PIN = 6; //D6
const int ECHO_PIN = 7; //D7
SR04 front(TRIGGER_PIN, ECHO_PIN, 10);


void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(front.getDistance());
  delay(100);
}

