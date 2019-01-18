/**
   This sketch can be used to "calibrate" the motors that are connected to the motor shield.
   Particularly, the poles of the brushed DC motors should be connected on each terminal on the shield.
   However, that by itself does not guarantee that the motors will be spinning towards the intended directions.

   Upload the following code and open the serial monitor.
   Then the on-screen instructions, will guide you step by step, on how to connect the various poles in order to
   have all motors spinning towards the correct direction.
*/

#include <Smartcar.h>

BrushedMotor leftMotor(8, 10, 9);
BrushedMotor rightMotor(12, 13, 11);
DifferentialControl control(leftMotor, rightMotor);

SimpleCar car(control);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println(F("\n========="));
  Serial.println(F("Welcome to the Smartcar platform!"));
  Serial.println(F("========="));
  delay(1500);
  Serial.println(F("This sketch will help you set the connections of the motors on the Smartcar shield"));
  delay(1500);
  Serial.println(F("To begin with: Connect the motors to the screw terminals. One motor on every terminal. The order of the cables does not matter right now."));
  while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s)
  Serial.println(F("Type a character and press enter when you are done."));
  delay(100);
  while (!Serial.available()); //wait for a character
  while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s)
  Serial.println(F("Good! Now we are going to try and make the motors to run FORWARD."));
  delay(1500);
  Serial.println(F("You should lift the car in the air, so it does not drive away while we are testing the motors."));
  delay(1500);
  while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s)
  Serial.println(F("OK, I will try to move the wheels forward for 6 seconds. Observe the direction of each motor and note which ones were NOT moving forward"));
  Serial.println(F("Are you ready? Type any character and press enter when you are done."));
  delay(100);
  while (!Serial.available()); //wait for a character
  while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s) by reading the character(s)
  while (true) {
    delay(500);
    car.setSpeed(80);
    delay(6000);
    car.setSpeed(0);
    Serial.println(F(">>>> If there were some motors not turning towards the correct direction, FLIP the respective cables in the terminals!"));
    Serial.println(F("Alternatively, change the two first arguments of the BrushedMotor constructor, e.g. leftMotor(10, 8, 9);"));
    delay(100);
    Serial.println(F("Repeat this until every wheel is turning FORWARD!"));
    delay(1500);
    while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s) by reading the character(s)
    Serial.println(F("Do you want to try again? Press 'y' for YES or 'n' for NO and then enter."));
    boolean repeat = false;
    while (true) {
      while (!Serial.available()); //wait for a character
      if (Serial.available()) {
        String input  = Serial.readStringUntil('\n');
        if (input.startsWith("y")) {
          repeat = true;
          break;
        } else if (input.startsWith("n")) {
          break;
        }
      }
    }
    if (!repeat) {
      break;
    }
    Serial.println(F("I will try to move the wheels forward for 6 seconds. Observe the direction of each motor and note which ones were NOT moving forward"));
  }
  Serial.println(F("Great! By now then you should have the motors spinning in the correct direction. Now, let's see if the car TURNS towards the correct direction also!"));
  delay(1500);
  Serial.println(F("OK, I will try to move the RIGHT wheels BACKWARD for 6 seconds. Verify that the RIGHT wheels spin towards the intended direction."));
  Serial.println(F("Are you ready? Type any character and press enter when you are done."));
  delay(100);
  while (!Serial.available()); //wait for a character
  while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s) by reading the character(s)
  while (true) {
    delay(500);
    car.overrideMotorSpeed(0, -80);
    delay(6000);
    car.overrideMotorSpeed(0, 0);
    while (Serial.available()) Serial.read(); //empty the input buffer by reading the character(s) by reading the character(s)
    Serial.println(F("Do you want to try again? Press 'y' for YES or 'n' for NO and then enter."));
    delay(1500);
    boolean repeat = false;
    while (true) {
      while (!Serial.available()); //wait for a character
      if (Serial.available()) {
        String input  = Serial.readStringUntil('\n');
        if (input.startsWith("y")) {
          repeat = true;
          break;
        } else if (input.startsWith("n")) {
          break;
        }
      }
    }
    if (!repeat) {
      break;
    }
  }
  Serial.println(F("If the RIGHT side motors were NOT spinning backwards, then you have two options:"));
  delay(100);
  Serial.println(F("a) Change the orientation of the arduino and the shield. Then repeat this process to set the motors again."));
  delay(100);
  Serial.println(F("b) Change the arguments of the leftMotor to the ones of the rightMotor and vice versa."));
  delay(1500);
  Serial.println(F("That was it! Happy hacking!"));
}

void loop() {
}
