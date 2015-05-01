#include <AndroidCar.h>
#include <Servo.h>
#include <Netstrings.h>
#include <Wire.h>
#include "CarVariables.h"


Car car(SERVO_PIN, ESC_PIN); //steering, esc
Sonar frontSonar, frontRightSonar, rearSonar;
Sharp_IR sideFrontIR, sideRearIR, rearIR;
Odometer encoder;
Gyroscope gyro;

const short comFrequency = 100;
unsigned long previousTransmission = 0;

void setup() {
  car.begin();
  frontSonar.attach(FRONT_US_TRIG_PIN, FRONT_US_ECHO_PIN);
  frontRightSonar.attach(FRONT_RIGHT_US_TRIG_PIN, FRONT_RIGHT_US_ECHO_PIN);
  rearSonar.attach(REAR_US_TRIG_PIN, REAR_US_ECHO_PIN);
  sideFrontIR.attach(SIDE_FRONT_IR_PIN);
  sideRearIR.attach(SIDE_REAR_IR_PIN);
  rearIR.attach(REAR_IR_PIN);
  encoder.attach(ENCODER_DIG_PIN);
  encoder.begin(); //start counting
  gyro.attach();
  gyro.begin(); //start measuring
  Serial2.begin(9600);
  Serial2.setTimeout(200);
  Serial.begin(9600);
}

void loop() {
  handleInput();
  gyro.update();
  transmitSensorData();
}

void transmitSensorData() {
  if (millis() - previousTransmission > comFrequency) {
    String out;
    out = "US1-";
    out += frontSonar.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "US2-";
    out += frontRightSonar.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "US3-";
    out += rearSonar.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "IR1-";
    out += sideFrontIR.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "IR2-";
    out += sideRearIR.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "IR3-";
    out += rearIR.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "EN-";
    out += encoder.getDistance();
    Serial2.println(encodedNetstring(out));
    out = "HE-";
    out += abs(gyro.getAngularDisplacement());
    Serial2.println(encodedNetstring(out));
    previousTransmission = millis();
  }
}

void handleInput() {
  if (Serial2.available()) {
    String input = decodedNetstring(Serial2.readStringUntil(','));
    Serial.println(input);
    if (input.startsWith("m")){
      int throttle = input.substring(1).toInt();
      car.setSpeed(throttle);
    }else if (input.startsWith("t")){
      int degrees = input.substring(1).toInt();
      car.setSteeringWheel(degrees);
    } else if (input.startsWith("h")){
      gyro.begin();
  }else{
      Serial2.println(encodedNetstring("Bad input"));  
    }
  }
}
