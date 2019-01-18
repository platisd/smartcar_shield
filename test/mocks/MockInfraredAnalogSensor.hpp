#pragma once

#include "gmock/gmock.h"

#include "InfraredAnalogSensor.hpp"

class MockInfraredAnalogSensor : public InfraredAnalogSensor
{
public:
    MockInfraredAnalogSensor(Runtime& runtime)
        : InfraredAnalogSensor(runtime)
    {
    }

    MOCK_METHOD0(getDistance, unsigned int());
};
