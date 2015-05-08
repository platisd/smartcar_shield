/* The running sketch on the Android Autonomous Vehicle by Team Pegasus
*  Author: Dimitris Platis
*/
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

const unsigned short COM_FREQ = 100;
unsigned long previousTransmission = 0;

const unsigned short OVERRIDE_TIMEOUT = 3000;
unsigned long overrideRelease = 0;
boolean overrideTriggered = false;

unsigned long prevCheck = 0;
const unsigned short LEDrefreshRate = 200;

Razorboard razor;

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
  razor.attach(&Serial3);
  pinMode(BT_STATE_PIN, INPUT);
  Serial2.begin(9600);
  Serial2.setTimeout(200);
}

void loop() {
  handleOverride(); //look for an override signal and if it exists disable bluetooth input
  handleInput();
  updateLEDs();
  gyro.update();
  transmitSensorData();
}

void updateLEDs() {
  if (millis() - prevCheck > LEDrefreshRate) {
    if (overrideTriggered) { //if override is triggered
      Serial3.print('m');
    } else {  //if override is NOT triggered
      if (!car.getSpeed()) { //if car is immobilized
        Serial3.print('s');
      } else { //if car is running
        if (car.getAngle() > 0) { //if car turns right
          Serial3.print('r');
        } else if (car.getAngle() < 0) { //if car turns left
          Serial3.print('l');
        } else { //if car goes straight
          Serial3.print('i');
        }
      }
    }
    prevCheck = millis();
  }
}

void handleOverride() {
  if (int override = pulseIn(OVERRIDE_SIGNAL_PIN, HIGH, MAX_WAVELENGTH)) {
    overrideRelease = millis() + OVERRIDE_TIMEOUT; //time to re-enable Serial communication
    overrideTriggered = true;
  }
}

void transmitSensorData() {
  if (bluetoothConnected && (millis() - previousTransmission > COM_FREQ)) {
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
    out = "RZR-";
    out += razor.getLatestHeading();
    Serial2.println(encodedNetstring(out));
    previousTransmission = millis();
  }

}

void handleInput() {
  if (!overrideTriggered || (millis() > overrideRelease)) {
    if (overrideTriggered) { //this state is only entered when the OVERRIDE_TIMEOUT is over
      overrideTriggered = false;
      //after going out of the override mode, set speed and steering to initial position
      car.setSpeed(0);
      car.setSteeringWheel(0);
    }
    if (Serial2.available()) {
      String input = decodedNetstring(Serial2.readStringUntil(','));
      if (input.startsWith("m")) {
        int throttle = input.substring(1).toInt();
        car.setSpeed(throttle);
      } else if (input.startsWith("t")) {
        int degrees = input.substring(1).toInt();
        car.setSteeringWheel(degrees);
      } else if (input.startsWith("h")) {
        gyro.begin();
      } else {
        Serial2.println(encodedNetstring("Bad input"));
      }
    }
  } else { //override mode
    unsigned short servoFreq = pulseIn(OVERRIDE_SERVO_PIN, HIGH, MAX_STEERING_WAVELENGTH);
    int throttleFreq = pulseIn(OVERRIDE_THROTTLE_PIN, HIGH, MAX_STEERING_WAVELENGTH);
    //handle override servo
    if (servoFreq) { //if you get 0, ignore it as it is between the pulses
      if (abs(servoFreq - NEUTRAL_FREQUENCY) < OVERRIDE_FREQ_TOLERANCE) {
        car.setSteeringWheel(0);
      } else {
        if (servoFreq > NEUTRAL_FREQUENCY) {
          car.setSteeringWheel(OVERRIDE_STEER_RIGHT);
        } else {
          car.setSteeringWheel(OVERRIDE_STEER_LEFT);
        }
      }
    }
    //handle override throttle
    if (throttleFreq) {
      if (abs(throttleFreq - NEUTRAL_FREQUENCY) < OVERRIDE_FREQ_TOLERANCE) {
        car.setSpeed(0);
      } else {
        if (throttleFreq > NEUTRAL_FREQUENCY) {
          car.setSpeed(OVERRIDE_FORWARD_SPEED);
        } else {
          car.setSpeed(OVERRIDE_BACKWARD_SPEED);
        }
      }
    }
    while (Serial2.read() != -1); //discard incoming data while on override
  }
}
