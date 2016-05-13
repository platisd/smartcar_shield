#include <Smartcar.h>

const unsigned short GAIN = 0x1F; //maximum gain
const unsigned short RANGE = 0x07; //7 for 34 centimeters
SRF08 front;

void setup() {
  front.attach(112);
 // front.setGain(GAIN);
 // front.setRange(RANGE);
 // front.setPingDelay(8); //uncomment if u want to use custom measurement range
  Serial.begin(9600);
}

void loop() {
  Serial.print("Distance: ");
  Serial.println(front.getDistance());
}
