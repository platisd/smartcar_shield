#include "GP2Y0A02.hpp"

namespace
{
const auto kMinDistance = 25;  // GP2Y0A02's minimum distance
const auto kMaxDistance = 120; // GP2Y0A02's maximum distance
} // namespace

GP2Y0A02::GP2Y0A02(uint8_t pin, Runtime& runtime)
    : InfraredAnalogSensor(runtime)
    , kPin{ pin }
    , mRuntime(runtime)
{
}

unsigned int GP2Y0A02::getDistance()
{
    auto analogReading = mRuntime.getAnalogPinState(kPin);
    auto result        = 9462 / (analogReading - 16.92);

    return (result >= kMinDistance && result <= kMaxDistance ? result : 0);
}
