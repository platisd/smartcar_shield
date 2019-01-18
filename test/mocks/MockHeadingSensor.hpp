#pragma once

#include "gmock/gmock.h"

#include "HeadingSensor.hpp"

class MockHeadingSensor : public HeadingSensor
{
public:
    MOCK_METHOD0(getHeading, unsigned int());
    MOCK_METHOD0(update, void());
};
