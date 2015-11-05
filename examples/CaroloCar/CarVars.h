#ifndef CarVars_h
#define CarVars_h

#define SERVO_PIN 8
#define ESC_PIN 9

#define US_REAR_ADDRESS 0x73
#define US_FRONT_ADDRESS 0x70

#define US_GAIN 0x1F //maximum gain
#define US_RANGE 0x07 //7 for 34 centimeters
#define US_DELAY 6 //this is connected to maximum gain, please refer to the datasheet

#define IR_REAR_LEFT_PIN A1
#define IR_REAR_RIGHT_PIN A2
#define IR_MIDDLE_REAR_PIN A0
#define IR_MIDDLE_FRONT_PIN A3

#define ENCODER_LEFT_PIN 2
#define ENCODER_RIGHT_PIN 3

#define OVERRIDE_THROTTLE_PIN A8
#define OVERRIDE_SERVO_PIN A9

#define NEUTRAL_FREQUENCY 1500
#define OVERRIDE_FORWARD_SPEED 0.3
#define OVERRIDE_BACKWARD_SPEED -0.3
#define OVERRIDE_STEER_RIGHT 40
#define OVERRIDE_STEER_LEFT -40
#define OVERRIDE_FREQ_TOLERANCE 200
#define MAX_OVERRIDE_FREQ 2100

#endif
