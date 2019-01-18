#pragma once

#include "gmock/gmock.h"

#include "Control.hpp"

class MockControl : public Control
{
public:
    MOCK_METHOD1(setSpeed, void(int speed));
    MOCK_METHOD1(setAngle, void(int angle));
    MOCK_METHOD2(overrideMotorSpeed, void(int firstMotorSpeed, int secondMotorSpeed));
};
