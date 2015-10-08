#include <SoftwareSerial.h>
#include "variables.h"

SoftwareSerial serialInput(RX_PIN, UNUSED_TX_PIN); //rx,tx

State currentState = STOP;
boolean stateChanged = false;
const uint8_t LED_TOGGLE_STATES[5][2] = { //array to hold the various states the LEDs can be. Each row represents one LED, in accordance to State values, as enumerated in variables.h
  {HIGH, LOW},
  {HIGH, HIGH}, //the stop lights are always high
  {LOW, LOW}, //not used
  {HIGH, LOW},
  {HIGH, LOW}
}; //for example, using  digitalWrite(RIGHT, LED_TOGGLE_STATES[RIGHT][0]) would set the RIGHT pin to its first state (HIGH in this case)
boolean ledToggleVal = false; //controls whether the respective pin will be LOW or HIGH. for example LED_TOGGLE_STATES[RIGHT][ledToggleVal] when ledToggleVal is false, means that when used in a digital write, the specified pin (RIGHT) should be set to HIGH
boolean ledToggled = false; //flag to indicate whether there has been a LED toggling. Used to determine whether it is necessary to digitalWrite or not (there is nothing "new" to be written)
unsigned long prevToggle = 0; //the moment the previous LED toggling occurred
const unsigned short LED_INTERVAL = 1000; //how often we should toggle the LED state (simply: how often we should blink the lights)

void setup() {
  serialInput.begin(38400); //higher BAUD RATE seems to crash it
  pinMode(STOP, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RC, OUTPUT);
  noLights(); //initialize all the LEDs to low
}

void loop() {
  handleInput(); //if there is input on the SoftwareSerial RX pin, match it to a specific state
  handleLEDs(); //decide on which LEDs to light up, depending on the current state and whether this just changed
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
        setState(MOVING_STRAIGHT);
        break;
    }
  }
}

void setState(State state) {
  if (currentState != state) stateChanged = true; //indicate that we have just changed the state
  currentState = state; //log down the new state
}

void handleLEDs() {
  if (stateChanged){ //if the state was just changed then we should re-initialize the led toggling sequence
    noLights(); //turn off all the LEDs, so whatever was previously on, will turn off now
    ledToggleVal = false; //start the ledToggle from the beginning
    ledToggled = true; //indicate the has been a led toggle
    prevToggle = millis(); //start the new toggle
    stateChanged = false; //acknowledge that we are aware of the state change and set it back to false
  }
  if (millis() > prevToggle + LED_INTERVAL){ //if it is time to toggle the state of the LED
    ledToggleVal = !ledToggleVal; //change the state
    ledToggled = true; //indicate the has been a led toggle
    prevToggle = millis(); //save the moment when this happened
  }
  if ((currentState != MOVING_STRAIGHT) && ledToggled){ //if we are not MOVING_FORWARD (thus all LEDs are off) then toggle the led state AND there has been a toggled LED
    digitalWrite(currentState, LED_TOGGLE_STATES[currentState][ledToggleVal]); //write the status of the led, on the appropriate pin (states match to pins)
    ledToggled = false; //indicate that the led has been toggled in this specific state, so this doesn't need to be run again
  }
}

void noLights() { //turn all lights off
  digitalWrite(RIGHT, LOW);
  digitalWrite(LEFT, LOW);
  digitalWrite(STOP, LOW);
  digitalWrite(RC, LOW);
}
