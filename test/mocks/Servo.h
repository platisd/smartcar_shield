/**
 * Servo "mock" class. Ideally our ServoMotor class would be using composition
 * to become an (Arduino) Servo motor. However, we want to make usage easier and
 * transparent to the user, so we are extending the Arduino Servo class.
 * The inheritance instead of composition makes things a little bit trickier
 * to mock and that is why we need a stub implementation of `Servo` that can get
 * injected with the mock which in turn we can set our expectations upon.
 */
#pragma once

#include <memory>
#include <stdexcept>
#include <stdint.h>

#include "gmock/gmock.h"

class MockServo
{
public:
    MOCK_METHOD3(attach, void(uint8_t pin, int maxPulseLength, int minPulseLength));
    MOCK_METHOD1(writeMicroseconds, void(int microseconds));
};

class Servo
{
public:
    void attach(uint8_t pin, int maxPulseLength, int minPulseLength)
    {
        mMockServo->attach(pin, maxPulseLength, minPulseLength);
    }

    void writeMicroseconds(int microseconds)
    {
        mMockServo->writeMicroseconds(microseconds);
    }

    std::shared_ptr<MockServo> mMockServo;
};
