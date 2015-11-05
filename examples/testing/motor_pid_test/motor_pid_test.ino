#include <CaroloCup.h>
#include <Servo.h>
#include <Wire.h>

#define SERVO_PIN 8
#define ESC_PIN 9
#define ENCODER_PIN 3

Car car(SERVO_PIN, ESC_PIN); //steering, esc
Odometer encoder;

void setup() {
  Serial.begin(9600); //initialize serial input
  Serial.setTimeout(200);
  encoder.attach(ENCODER_PIN);
  encoder.begin();
  car.begin(); //initialize the car
  car.enableCruiseControl(encoder);
  car.setSpeed(0);
}

void loop() {
  car.updateMotors();
  handleInput();
}

void handleInput() { //handle serial input if there is any
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("m")) {
      float throttle = input.substring(1).toFloat();
      Serial.println(throttle);
      car.setSpeed(throttle);
    } else if (input.startsWith("t")) {
      int degrees = input.substring(1).toInt();
      car.setAngle(degrees);
    } else if (input.startsWith("h")) {
      //     gyro.begin(GYRO_SAMPLING);
    } else {
      Serial.println("Bad input");
    }
  }
}
