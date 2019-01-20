#pragma once

#include "gmock/gmock.h"

#include "Odometer.hpp"

class MockOdometer : public Odometer
{
public:
    MOCK_METHOD2(attach, bool(uint8_t pin, void (*callback)()));
    MOCK_METHOD0(getDistance, long());
    MOCK_METHOD0(getSpeed, float());
    MOCK_METHOD0(isAttached, bool());
    MOCK_METHOD0(providesDirection, bool());
};
