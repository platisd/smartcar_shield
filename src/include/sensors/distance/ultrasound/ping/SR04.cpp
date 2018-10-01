/*
 *    SR04.cpp - Handles the ultra sound (HC-SR04 & SRF05) sensors of the Smartcar
 *    Author: Dimitris Platis
 *    SR04 class is essentially a stripped-down version of the NewPing library by Tim Eckel adjusted to Smartcar needs
 *     Get original library at: http://code.google.com/p/arduino-new-ping/
 *     License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
 */
#include "SR04.hpp"

namespace
{
// Microseconds it takes sound to travel round-trip 1cm (2cm total)
const auto kRountripInMicros = 57;
} // namespace

SR04::SR04(Runtime& runtime, unsigned int maxDistance)
    : mRuntime{ runtime }
    , mMaxDistance{ maxDistance }
    , mTriggerBit{ 0 }
    , mEchoBit{ 0 }
    , mTriggerOutput{ 0 }
    , mTriggerMode{ 0 }
    , mEchoInput{ 0 }
    , mMaxEchoTime{ 0 }
    , mMaxTime{ 0 }
{
}

void SR04::attach(uint8_t triggerPin, uint8_t echoPin)
{
    // Get the port register bitmask for the trigger pin.
    mTriggerBit = digitalPinToBitMask(triggerPin);
    // Get the port register bitmask for the echo pin.
    mEchoBit = digitalPinToBitMask(echoPin);

    // Get the output port register for the trigger pin.
    mTriggerOutput = portOutputRegister(digitalPinToPort(triggerPin));
    // Get the input port register for the echo pin.
    mEchoInput = portInputRegister(digitalPinToPort(echoPin));

    // Get the port mode register for the trigger pin.
    mTriggerMode = static_cast<uint8_t*>(portModeRegister(digitalPinToPort(triggerPin)));

    // Calculate the maximum distance in uS.
    mMaxEchoTime = mMaxDistance * kRountripInMicros + (kRountripInMicros / 2);
    // Set trigger pin to output.
    *mTriggerMode |= mTriggerBit;
}

unsigned int SR04::ping()
{
    static const auto kNoEcho = 0;

    if (!ping_trigger())
    {
        // Trigger a ping, if it returns false, return kNoEcho to the calling function.
        return kNoEcho;
    }

    // Wait for the ping echo.
    while (*mEchoInput & mEchoBit)
    {
        if (micros() > mMaxTime)
        {
            // Stop the loop and return kNoEcho (false) if we're beyond the set maximum distance.
            return kNoEcho;
        }
    }

    // Calculate ping time, 5uS of overhead.
    return micros() - (mMaxTime - mMaxEchoTime) - 5;
}

unsigned int SR04::getDistance()
{
    auto echoTime = ping();

    return (max((echoTime + kRountripInMicros / 2) / kRountripInMicros,
                static_cast<unsigned int>(echoTime ? 1 : 0))); // Convert uS to centimeters.
}

bool SR04::ping_trigger()
{
    // Maximum uS it takes for sensor to start the ping
    static const unsigned int kMaxSensorDelayInMicros = 5800;

    // Set the trigger pin low, should already be low, but this will make sure it is.
    *mTriggerOutput &= ~mTriggerBit;
    // Wait for pin to go low, testing shows it needs 4uS to work every time.
    delayMicroseconds(4);
    // Set trigger pin high, this tells the sensor to send out a ping.
    *mTriggerOutput |= mTriggerBit;
    // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
    delayMicroseconds(10);
    // Set trigger pin back to low.
    *mTriggerOutput &= ~mTriggerBit;
    // Set a timeout for the ping to trigger.
    mMaxTime = micros() + kMaxSensorDelayInMicros;

    while (*mEchoInput & mEchoBit && micros() <= mMaxTime)
    {
        // Wait for echo pin to clear.
    }

    // Wait for ping to start.
    while (!(*mEchoInput & mEchoBit))
    {
        if (micros() > mMaxTime)
        {
            // Something went wrong, abort.
            return false;
        }
    }

    // Ping started, set the timeout
    mMaxTime = micros() + mMaxEchoTime;
    // Ping started successfully
    return true;
}
