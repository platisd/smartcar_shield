#include <SoftwareSerial.h>
#include "variables.h"

SoftwareSerial serialInput(RX_PIN, 3); //rx,tx

State currentState = IDLE;
unsigned long prevCheck = 0;
const unsigned short refreshRate = 100;

void setup() {
  serialInput.begin(9600);
  pinMode(STOP, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RC, OUTPUT);
}

void loop() {
  handleInput();
  hanleLEDs(currentState);
}

void handleInput() {
  if (serialInput.available()) {
    char inChar;
    while (serialInput.available()) inChar = serialInput.read(); //read through the buffer and keep the last character disregarding what came in the meanwhile
    switch (inChar) {
      case 'l':
        setState(LEFT);
        break;
      case 'r':
        setState(RIGHT);
        break;
      case 'm':
        setState(RC);
        break;
      case 's':
        setState(STOP);
        break;
      default:
        setState(IDLE);
        break;
    }
  }
}

void setState(State state) {
  currentState = state;
}

void hanleLEDs(State state) {
  if (millis() - prevCheck > refreshRate) {
    prevCheck = millis();
    switch (state) {
      case STOP:
        stopLights();
        break;
      case IDLE:
        noLights();
        break;
      default: //must be a blinker then (Left, right, rc)
        blinkers(state);
        break;
    }
  }
}

void blinkers(State LED) {
  noLights(); //clear all previous blinkers
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}

void stopLights() {
  noLights(); //clear all previous blinkers
  digitalWrite(STOP, HIGH);
}

void noLights() {
  digitalWrite(RIGHT, LOW);
  digitalWrite(LEFT, LOW);
  digitalWrite(STOP, LOW);
  digitalWrite(RC, LOW);
}
