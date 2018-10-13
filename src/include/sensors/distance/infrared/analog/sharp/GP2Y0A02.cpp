#include "GP2Y0A02.hpp"

namespace
{
const auto kMinDistance = 25;  // GP2Y0A02's minimum distance
const auto kMaxDistance = 120; // GP2Y0A02's maximum distance
} // namespace

GP2Y0A02::GP2Y0A02(Runtime& runtime)
    : InfraredAnalogSensor(runtime)
    , mRuntime{ runtime }
{
}

unsigned int GP2Y0A02::getDistance()
{
    if (!mSensorAttached)
    {
        return -1;
    }
    auto analogReading     = mRuntime.getAnalogPinState(mPin);
    auto result            = 9462 / (analogReading - 16.92);

    return (result >= kMinDistance && result <= kMaxDistance ? result : 0);
}
