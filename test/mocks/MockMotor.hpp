#pragma once

#include "gmock/gmock.h"

#include "Motor.hpp"

class MockMotor : public Motor
{
public:
    MOCK_METHOD1(setSpeed, void(int speed));
};
