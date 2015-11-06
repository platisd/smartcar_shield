#include <Wire.h>
#include <Servo.h>
#include <CaroloCup.h>
#include "CarVars.h"

#define DEBUG //comment this line out for protobuffer output
#if 1 //preprocessor bug workaround, do not remove if you want the #ifndef DEBUG to work
__asm volatile ("nop");
#endif
#ifndef DEBUG //if we are not in debug, include the protobuffer libraries and declare variables
#include <pb_encode.h>
#include <pb_decode.h>
#include <messageproto.pb.h>
const unsigned short flagEND = 19;
const unsigned short flagESC = 125;
const unsigned short varXOR  = 32;
const unsigned short BUFFER_SIZE = 32;
uint8_t enc_buffer[32];
uint8_t dec_buffer[32];
size_t message_length;
boolean status;
#else //if we are in debug, we'll use the Netstrings library for encoding
#include <Netstrings.h>
#endif

Odometer encoderLeft, encoderRight;
Gyroscope gyro;
Car car(SERVO_PIN, ESC_PIN); //steering, esc
SRF08 frontSonar, rearSonar;
Sharp_IR rearLeftIR, rearRightIR, middleRearIR, middleFrontIR;

const unsigned short COM_FREQ = 60;
unsigned long previousTransmission = 0;

unsigned long prevCheck = 0; //for the LEDs
const unsigned short LEDrefreshRate = 200;

const unsigned short OVERRIDE_TIMEOUT = 500;
unsigned long overrideRelease = 0; //variable to hold WHEN the override should be lifted
volatile boolean overrideTriggered = false; //volatile since we are accessing it inside an ISR

volatile unsigned long throttleSignalStart = 0;
volatile boolean throttleSignalPending = false;
volatile unsigned int throttleSignalFreq = 0;

volatile unsigned long steeringSignalStart = 0;
volatile boolean steeringSignalPending = false;
volatile unsigned int steeringSignalFreq = 0;

volatile uint16_t qualityControl = 0; //if this byte is 1111111111111111, that means the measurements we received were of good quality (controller is turned on)
unsigned int throttleFreq = 0;
unsigned int servoFreq = 0;

void setup() {
  car.begin();
  frontSonar.attach(US_FRONT_ADDRESS);
  frontSonar.setGain(US_GAIN);
  frontSonar.setRange(US_RANGE);
  frontSonar.setPingDelay(US_DELAY);
  rearSonar.attach(US_REAR_ADDRESS);
  rearSonar.setGain(US_GAIN);
  rearSonar.setRange(US_RANGE);
  rearSonar.setPingDelay(US_DELAY);
  rearLeftIR.attach(IR_REAR_LEFT_PIN);
  rearRightIR.attach(IR_REAR_RIGHT_PIN);
  middleRearIR.attach(IR_MIDDLE_REAR_PIN);
  middleFrontIR.attach(IR_MIDDLE_FRONT_PIN);
  encoderLeft.attach(ENCODER_LEFT_PIN);
  encoderLeft.begin();
  encoderRight.attach(ENCODER_RIGHT_PIN);
  encoderRight.begin();
  setupChangeInterrupt(OVERRIDE_THROTTLE_PIN);
  setupChangeInterrupt(OVERRIDE_SERVO_PIN);
  gyro.attach();
  gyro.begin(); //default 80ms
  delay(500); //wait a bit for the esc
  car.enableCruiseControl(encoderLeft);
  car.setSpeed(0);
  Serial.begin(115200); //to HLB
  Serial.setTimeout(200); //set a timeout so Serial.readStringUntil dies after the specified amount of time
  Serial3.begin(9600); //to LED driver
}

void loop() {
  handleOverride(); //look for an override signal and if it exists disable serial input from the HLB
  handleInput(); //look for a serial input if override is not triggered and act accordingly
  updateLEDs(); //update LEDs depending on the mode we are currently in
  gyro.update(); //integrate gyroscope's readings
  car.updateMotors();
  transmitSensorData(); //fetch and transmit the sensor data in the correct intervals
}

void handleOverride() {
  boolean qualityCheck = (qualityControl == 0xFFFF); //true if the last 16 measurements were valid
  if (qualityCheck) { //good quality, means that the RC controller is turned on, therefore we should go on override mode
    overrideTriggered = true;
    overrideRelease = millis() + OVERRIDE_TIMEOUT; //specify the moment in the future to re-enable Serial communication
  } else { //this means that at least one of the last 16 measurements was not valid, therefore we consider the signal not to be of good quality (RC controller is turned off)
    throttleSignalPending = false; //indicate that loop() has processed/disregarded the throttle signal
    steeringSignalPending = false; //indicate that loop() has read/disregarded the servo signal
  }
  if (overrideTriggered) { //if override is triggered, then you can consider signals from the channels
    boolean _throttleSignalPending = throttleSignalPending;
    if (_throttleSignalPending) {
      throttleFreq = throttleSignalFreq; //save the throttle's frequency
      throttleSignalPending = false; //indicate that loop() has processed the throttle signal
    }
    boolean _steeringSignalPending = steeringSignalPending;
    if (_steeringSignalPending) { //if there is something to be processed
      servoFreq = steeringSignalFreq; //save the steering's frequency
      steeringSignalPending = false; //indicate that loop() has read the servo signal
    }
  }
}

void handleInput() {
  if (!overrideTriggered || (millis() > overrideRelease)) {
    if (overrideTriggered) { //this state is only entered when the OVERRIDE_TIMEOUT is over
      overrideTriggered = false;
      car.setSpeed(0); //after going out of the override mode, set speed and steering to initial position
      car.setAngle(0);
    }
    if (Serial.available()) {
#ifdef DEBUG //if we are in debug mode, use plain text with netstrings
      String input = decodedNetstring(Serial.readStringUntil(','));
      if (input.startsWith("m")) {
        int throttle = input.substring(1).toInt();
        car.setSpeed(throttle);
      } else if (input.startsWith("t")) {
        int degrees = input.substring(1).toInt();
        car.setAngle(degrees);
      } else if (input.startsWith("b")) {
        car.stop();
      } else if (input.startsWith("h")) {
        gyro.begin();
      } else {
        Serial.println(encodedNetstring("Bad input"));
      }
#else //use protobuffer
      String input = Serial.readStringUntil(flagEND);
      for (int i = 0, pos = 0; i < input.length(); i++, pos++) {
        if (input[i] == flagESC) {
          i++;
          dec_buffer[pos] = (int)input[i] ^ varXOR;
        } else {
          dec_buffer[pos] = input[i];
        }
      }
      boolean protoDec;
      Control message;
      pb_istream_t instream = pb_istream_from_buffer(dec_buffer, BUFFER_SIZE);
      protoDec = pb_decode(&instream, Control_fields, &message);
      if (protoDec) { //if it's a valid protopacket
        car.setSpeed(message.acceleration);
        car.setAngle(message.steering);
      }
#endif
    }
  } else { //we are in override mode now
    //handle override steering
    if (servoFreq && (servoFreq < MAX_OVERRIDE_FREQ) && (servoFreq > MIN_OVERRIDE_FREQ)) { //if you get 0, ignore it as it is not a valid value
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
    if (throttleFreq && (throttleFreq < MAX_OVERRIDE_FREQ) && (throttleFreq > MIN_OVERRIDE_FREQ)) {
      short diff = throttleFreq - NEUTRAL_FREQUENCY;
      if (abs(diff) < OVERRIDE_FREQ_TOLERANCE) { //if the signal we received is close to the idle frequency, then we assume it's neutral
        car.setSpeed(0);
      } else {
        if (throttleFreq < NEUTRAL_FREQUENCY) { //turn right if the value is smaller (that's the way it is with this receiver) than the idle frequency
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
#ifdef DEBUG //if we are in debug mode, use plain text with netstrings
    String out;
    out = "US1-";
    out += frontSonar.getDistance();
    out += ".US2-";
    out += rearSonar.getDistance();
    out += ".IR1-";
    out += rearLeftIR.getDistance();
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
    out += ".GYR-";
    out += gyro.getAngularDisplacement();
    Serial.println(encodedNetstring(out));
#else //use protobuffer
    Sensors message;
    message.usFront = frontSonar.getDistance();
    message.usRear = rearSonar.getDistance();
    message.irFrontRight = middleFrontIR.getDistance();
    message.irRearRight = middleRearIR.getDistance();
    message.irBackLeft = rearLeftIR.getDistance();
    message.irBackRight = rearRightIR.getDistance();
    message.wheelFrontLeft = encoderLeft.getDistance();
    message.wheelRearRight = encoderRight.getDistance();
    pb_ostream_t outstream = pb_ostream_from_buffer(enc_buffer, sizeof(enc_buffer));
    status = pb_encode(&outstream, Sensors_fields, &message);
    message_length = outstream.bytes_written;
    if (status) { //if valid send the protobytes
      for (int i = 0; i < message_length; i++) {
        if (enc_buffer[i] == flagEND || enc_buffer[i] == flagESC) {
          Serial.write(flagESC);
          Serial.write(enc_buffer[i]^varXOR);
        } else {
          Serial.write(enc_buffer[i]);
        }
      }
      Serial.write(flagEND);
    }
#endif
    previousTransmission = millis();
  }
}

void setupChangeInterrupt(const unsigned short pin) { //a method to setup change interrupts on non external interrupt pins
  pinMode(pin, INPUT); //set the pin as input
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

//the interrupt service routine for pins A8 until A15 on Arduino mega
ISR (PCINT2_vect) {
  unsigned short throttle = digitalRead(OVERRIDE_THROTTLE_PIN);
  unsigned short steering = digitalRead(OVERRIDE_SERVO_PIN);
  if (throttle) { //we are at the beginning of a throttle pulse
    if (!throttleSignalStart) { //it's the beginning of a pulse, if it is HIGH and we have not already started measuring (throttleSignalStart == 0)
      throttleSignalStart = micros(); //log down the microseconds at the beginning of the pulse
    }
  } else { //we are at the end of the throttle pulse
    if (throttleSignalStart && !throttleSignalPending) { //if the throttle signal has been started AND there is no throttle signal pending to be processed by loop()
      throttleSignalFreq = micros() - throttleSignalStart; //calculate the throttle signal's period
      throttleSignalStart = 0; //initialize the starting point of the measurement, so we do not go in here again, while the pulse is low
      throttleSignalPending = true; //signal loop() that there is a signal to handle
      qualityControl = qualityControl << 1;
      if ((throttleSignalFreq < MIN_OVERRIDE_FREQ)  || (throttleSignalFreq > MAX_OVERRIDE_FREQ)) { //since we are using an analog RC receiver, there is a lot of noise, usually under the frequency of 900 or over 2000
        qualityControl |= 0; //put a 0 bit in the end of qualityControl byte
      } else { //this means that is a valid looking signal
        qualityControl |= 1; //put a 1 bit in the end of qualityControl byte
      } //we do not need to do this for both the channels we have
    }
  }
  if (steering) { //we could be at the beginning of a steering pulse
    if (!steeringSignalStart) { //if we are already measuring something, that means this is NOT the beginning of a pulse
      steeringSignalStart = micros(); //get the current time in microseconds, ONLY IF this is really the beginning of a pulse and we weren't already measuring
    }
  } else { //we are at the end of a steering pulse
    if (steeringSignalStart && !steeringSignalPending) { //if the steering signal for the servo has started aAND there is no servo signal pending to be processed by loop()
      steeringSignalFreq = micros() - steeringSignalStart;
      steeringSignalStart = 0; //initialize the starting point of the measurement, so we do not go in here again, while the pulse is low
      steeringSignalPending = true; //signal loop() that there is a signal to handle
    }
  }
}
