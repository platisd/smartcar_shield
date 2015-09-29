#include <Wire.h>
#include <Servo.h>
#include <CaroloCup.h>
#include <Netstrings.h>
#include "CarVars.h"

Car car(SERVO_PIN, ESC_PIN); //steering, esc
SRF08 frontSonar, rearSonar;
Sharp_IR rearLeftIR, rearRightIR, middleRearIR, middleFrontIR;
Odometer encoderLeft, encoderRight;

const unsigned short COM_FREQ = 60;
unsigned long previousTransmission = 0;

unsigned long prevCheck = 0; //for the LEDs
const unsigned short LEDrefreshRate = 200;

volatile unsigned long overrideSignalStart = 0;
volatile boolean overrideSignalPending = false;
volatile int overrideSignalFreq = 0;

const unsigned short OVERRIDE_TIMEOUT = 3000;
unsigned long overrideRelease = 0; //variable to hold WHEN the override should be lifted
volatile boolean overrideTriggered = false; //volatile since we are accessing it inside an ISR

volatile unsigned long throttleSignalStart = 0;
volatile boolean throttleSignalPending = false;
volatile int throttleSignalFreq = 0;

volatile unsigned long steeringSignalStart = 0;
volatile boolean steeringSignalPending = false;
volatile int steeringSignalFreq = 0;

unsigned int throttleFreq = 0;
unsigned int servoFreq = 0;

void setup() {
  car.begin();
  frontSonar.attach(US_FRONT_ADDRESS);
  frontSonar.setGain(US_GAIN);
  frontSonar.setRange(US_RANGE);
  frontSonar.setPingDelay(6);
  rearSonar.attach(US_REAR_ADDRESS);
  rearSonar.setGain(US_GAIN);
  rearSonar.setRange(US_RANGE);
  rearSonar.setPingDelay(6);
  rearLeftIR.attach(IR_REAR_LEFT_PIN);
  rearRightIR.attach(IR_REAR_RIGHT_PIN);
  middleRearIR.attach(IR_MIDDLE_REAR_PIN);
  middleFrontIR.attach(IR_MIDDLE_FRONT_PIN);
  encoderLeft.attach(ENCODER_LEFT_PIN);
  encoderLeft.begin();
  encoderRight.attach(ENCODER_RIGHT_PIN);
  encoderRight.begin();
  pinMode(OVERRIDE_SIGNAL_PIN, INPUT);
  pinMode(OVERRIDE_THROTTLE_PIN, INPUT);
  pinMode(OVERRIDE_SERVO_PIN, INPUT);
  setupChangeInterrupt(OVERRIDE_SIGNAL_PIN);
  //  setupChangeInterrupt(RIGHT_IR_ARRAY);
  Serial.begin(9600); //to HLB
  Serial3.begin(9600); //to LED driver
}

void loop() {
  handleOverride();
  handleInput();
  updateLEDs();
  transmitSensorData();
}

void handleOverride() {
  noInterrupts();
  boolean _overrideSignalPending = overrideSignalPending;
  interrupts();
  if (_overrideSignalPending) {
    noInterrupts();
    short diff = overrideSignalFreq - OVERRIDE_SIGNAL_AVERAGE_FREQ;
    interrupts();
    if (abs(diff) < OVERRIDE_SIGNAL_TOLERANCE) { //there is a valid override signal
      overrideRelease = millis() + OVERRIDE_TIMEOUT; //time to re-enable Serial communication
      overrideTriggered = true;
    }
    noInterrupts();
    overrideSignalPending = false; //indicate that loop() has processed the override signal
    interrupts();
  }
  if (overrideTriggered) { //if override is triggered, then you can consider signals from the other channels
    noInterrupts();
    boolean _throttleSignalPending = throttleSignalPending;
    interrupts();
    if (_throttleSignalPending) {
      noInterrupts();
      throttleFreq = throttleSignalFreq; //save the throttle's frequency
      throttleSignalPending = false; //indicate that loop() has processed the throttle signal
      interrupts();
    }
    noInterrupts();
    boolean _steeringSignalPending = steeringSignalPending;
    interrupts();
    if (_steeringSignalPending) { //if there is something to be processed
      noInterrupts();
      servoFreq = steeringSignalFreq; //save the steering's frequency
      steeringSignalPending = false; //indicate that loop() has read the throttle signal
      interrupts();
    }
  }

}

void handleInput() {
  if (!overrideTriggered || (millis() > overrideRelease)) {
    if (overrideTriggered) { //this state is only entered when the OVERRIDE_TIMEOUT is over
      overrideTriggered = false;
      //after going out of the override mode, set speed and steering to initial position
      car.setSpeed(0);
      car.setAngle(0);
    }
    if (Serial.available()) {
      String input = decodedNetstring(Serial.readStringUntil(','));
      if (input.startsWith("m")) {
        int throttle = input.substring(1).toInt();
        car.setSpeed(throttle);
      } else if (input.startsWith("t")) {
        int degrees = input.substring(1).toInt();
        car.setAngle(degrees);
      } else if (input.startsWith("b")) {
        car.stop();
      } else {
        Serial.println(encodedNetstring("Bad input"));
      }
    }
  } else { //we are in override mode now
    //handle override steering
    if (servoFreq) { //if you get 0, ignore it as it is not a valid value
      short diff = servoFreq - NEUTRAL_FREQUENCY;
      if (abs(diff) < OVERRIDE_FREQ_TOLERANCE) { //if the signal we received is close to the idle frequency, then we assume it's neutral
        car.setAngle(0);
      } else { //if the difference between the signal we received and the idle frequency is big enough, only then move the servo
        if (servoFreq > NEUTRAL_FREQUENCY) { //turn right if the value is larger than the idle frequency
          car.setAngle(OVERRIDE_STEER_RIGHT);
        } else {
          car.setAngle(OVERRIDE_STEER_LEFT);//turn left if the value is smaller than the idle frequency
        }
      }
    }
    //handle override throttle
    if (throttleFreq) {
      short diff = throttleFreq - NEUTRAL_FREQUENCY;
      if (abs(diff) < OVERRIDE_FREQ_TOLERANCE) { //if the signal we received is close to the idle frequency, then we assume it's neutral
        car.setSpeed(0);
      } else {
        if (throttleFreq > NEUTRAL_FREQUENCY) { //turn right if the value is larger than the idle frequency
          car.setSpeed(OVERRIDE_FORWARD_SPEED);
        } else {
          car.setSpeed(OVERRIDE_BACKWARD_SPEED);
        }
      }
    }
    while (Serial.read() != -1); //discard incoming data while on override
  }
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

void transmitSensorData() {
  if (millis() - previousTransmission > COM_FREQ) {
    String out;
    out = "US1-";
    out += frontSonar.getDistance();
    out += ".US2-";
    out += rearSonar.getDistance();
    out += ".IR1-";
    out += rearLeftIR.getDistance(); //rearLeftIR, rearRightIR, middleRearIR, middleFrontIR;
    out += ".IR2-";
    out += rearRightIR.getDistance();
    out += ".IR3-";
    out += middleRearIR.getDistance();
    out += ".IR4-";
    out += middleFrontIR.getDistance();
    out += ".EN1-";
    out += encoderLeft.getDistance();
    out += ".EN2-";
    out += encoderRight.getDistance();
    Serial.println(encodedNetstring(out));
    previousTransmission = millis();
  }
}

void setupChangeInterrupt(unsigned short pin) { //a method to alternatively setup change interrupts on non external interrupt pins
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

//the interrupt service routine for pins A8 until A15 on Arduino mega
ISR (PCINT2_vect) {
  unsigned short override = digitalRead(OVERRIDE_SIGNAL_PIN);
  if (override) { //we are at the beginning of a control signal pulse
    overrideSignalStart = micros();
  } else { //we are at the end of the pulse
    if (overrideSignalStart && !overrideSignalPending) { //if an override's signal start has been detected AND there is no override signal pending to be processed by loop()
      overrideSignalFreq = micros() - overrideSignalStart; // the time of the falling edge MINUS the time of the rising edge, gives us the period of the signal
      overrideSignalStart = 0;
      overrideSignalPending = true; //there is an override signal to be processed pending by loop()
    }
  }
  if (overrideTriggered) { //if there is an override control signal, then take into consideration the two other channels
    unsigned short throttle = digitalRead(OVERRIDE_SIGNAL_PIN);
    unsigned short steering = digitalRead(OVERRIDE_SERVO_PIN);
    if (throttle) { //we are at the beginning of a throttle pulse
      throttleSignalStart = micros();
    } else { //we are at the end of the throttle pulse
      if (throttleSignalStart && !throttleSignalPending) { //if the throttle signal has been started AND there is no throttle signal pending to be processed by loop()
        throttleSignalFreq = micros() - throttleSignalStart; //calculate the throttle signal's period
        throttleSignalStart = 0;
        throttleSignalPending = true; //signal loop() that there is a signal to handle
      }
    }
    if (steering) { //we are at the beginning of a steering pulse
      steeringSignalStart = micros(); //get the current time in microseconds
    } else { //we are at the end of a steering pulse
      if (steeringSignalStart && !steeringSignalPending) { //if the steering signal for the servo has started aAND there is no servo signal pending to be processed by loop()
        steeringSignalFreq = micros() - throttleSignalStart;
        steeringSignalStart = 0;
        steeringSignalPending = true; //signal loop() that there is a signal to handle
      }
    }
  }
}
