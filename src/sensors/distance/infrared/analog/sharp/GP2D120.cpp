#include "GP2D120.hpp"

namespace
{
const auto kMinDistance = 5;  // GP2D120's minimum distance
const auto kMaxDistance = 25; // GP2D120's maximum distance
} // namespace

GP2D120::GP2D120(Runtime& runtime, uint8_t pin)
    : InfraredAnalogSensor(runtime)
    , kPin{ pin }
    , mRuntime(runtime)
{
}

unsigned int GP2D120::getDistance()
{
    auto analogReading = mRuntime.getAnalogPinState(kPin);
    // It's OK to surpress this clang-tidy warning since this is part of a magic (!) formula
    // NOLINTNEXTLINE(readability-magic-numbers)
    auto result = static_cast<unsigned int>((2914 / (analogReading + 5)) - 1);

    return (result >= kMinDistance && result <= kMaxDistance ? result : 0);
}
