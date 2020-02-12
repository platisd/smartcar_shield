#include <math.h>
#include <stdint.h>

#include "../../utilities/Utilities.hpp"
#include "DistanceCar.hpp"

namespace
{
const unsigned long kStopAttemptInterval = 100;
const uint8_t kMaxStopAttempts           = 3;
} // namespace

using namespace smartcarlib::constants::car;
using namespace smartcarlib::constants::control;
using namespace smartcarlib::utils;

DistanceCar::DistanceCar(Control& control, Odometer& odometer, Runtime& runtime)
    : SimpleCar(control)
    , mOdometerLeft(odometer)
    , mOdometerRight(odometer)
    , mRuntime(runtime)
{
}

DistanceCar::DistanceCar(Control& control,
                         Odometer& odometerLeft,
                         Odometer& odometerRight,
                         Runtime& runtime)
    : SimpleCar(control)
    , mOdometerLeft(odometerLeft)
    , mOdometerRight(odometerRight)
    , mRuntime(runtime)
{
}

long DistanceCar::getDistance()
{
    return areOdometersAttached() ? (mOdometerLeft.getDistance() + mOdometerRight.getDistance()) / 2
                                  : static_cast<long>(kOdometersNotAttachedError);
}

void DistanceCar::setSpeed(float speed)
{
    if (!areOdometersAttached())
    {
        SimpleCar::setSpeed(speed);
        return;
    }
    speed = getConstrain(
        speed, static_cast<float>(kMinControlSpeed), static_cast<float>(kMaxControlSpeed));
    // If odometers are directional and cruise control is enabled then we don't
    // need to do more since the speed controller will take care of the speed
    if (areOdometersDirectional() && mCruiseControlEnabled)
    {
        mTargetSpeed = speed;
        return;
    }
    // If we need to change direction of movement while the cruise control is
    // enabled, then we need to stop the car before proceeding to a different
    // direction since the car cannot determine which direction it is actually
    // moving.
    auto newSpeedAtOppositeDirection         = mTargetSpeed * speed < 0;
    auto changeOfDirectionWhileCruiseControl = mCruiseControlEnabled && newSpeedAtOppositeDirection;
    // Alternatively, we also need to stop the car if the supplied speed is `0`
    // and the car is not already stopped.
    auto carIsAlreadyStopped = areAlmostEqual(mTargetSpeed, static_cast<float>(kIdleControlSpeed));
    auto carWantsToStop      = areAlmostEqual(speed, static_cast<float>(kIdleControlSpeed));
    auto carShouldStop       = carWantsToStop && !carIsAlreadyStopped;
    if (changeOfDirectionWhileCruiseControl || carShouldStop)
    {
        brake();
        mTargetSpeed = speed;
        return;
    }
    // If the car does not need to stop then set the supplied speed to the motors
    // unless cruise control is enabled, in which case the speed adjustments will
    // be taken care by the speed controller.
    if (!mCruiseControlEnabled)
    {
        SimpleCar::setSpeed(speed);
    }
    mTargetSpeed = speed;
}

void DistanceCar::brake()
{
    // Apply the opposite speed than the one currently applied to the motors
    // until the car is moving slow enough and is practically stopped
    auto currentSpeed = mCruiseControlEnabled ? mPreviousControlledSpeed : mTargetSpeed;
    SimpleCar::setSpeed(-currentSpeed);
    // Determine when we are moving slow enough to stop applying speed towards
    // the opposite direction. We do not use the interrupt-based `getSpeed` from
    // the odometers since when there are no pulses there will be no speed
    // calculation.
    for (auto attempt = 0; attempt < kMaxStopAttempts; attempt++)
    {
        auto initialDistance = getDistance();
        mRuntime.delayMillis(kStopAttemptInterval);
        auto distanceTravelled = getDistance() - initialDistance;
        if (distanceTravelled == 0)
        {
            break;
        }
    }
    SimpleCar::setSpeed(static_cast<float>(kIdleControlSpeed));
    // Reset the speed controller variables since we have manually set the speed
    mPreviousControlledSpeed = 0;
    mIntegratedError         = 0;
    mPreviousError           = 0;
}

void DistanceCar::update()
{
    if (!areOdometersAttached() || !mCruiseControlEnabled)
    {
        return;
    }

    auto currentTime = mRuntime.currentTimeMillis();
    if (currentTime < mPreviousUpdate + mFrequency)
    {
        return;
    }
    mPreviousUpdate = currentTime;

    // Don't try to immobilize the car if odometers are not directional since we
    // have already done that when setting the speed
    if (!areOdometersDirectional() && mTargetSpeed == kIdleControlSpeed)
    {
        return;
    }
    auto actualSpeed = getSpeed();
    // If we do not have a way of determining direction of movement and the set
    // speed is negative, then we assume that the car is also moving backwards.
    // This assumption is based on the fact that if a speed towards the opposite
    // direction is supplied we should make first sure we are stopped before
    // changing direction.
    if (!areOdometersDirectional() && mTargetSpeed < 0)
    {
        actualSpeed = -actualSpeed;
    }
    auto controlledSpeed = controlMotorSpeed(mPreviousControlledSpeed, mTargetSpeed, actualSpeed);
    // In case direction readings are not supported by the odometers, we need to
    // ensure that runaway will not occur (see #5) due to our assumption that we
    // are moving at the same direction as our set speed.
    if (!areOdometersDirectional() && (controlledSpeed * actualSpeed < 0))
    {
        // If we are unable to determine the direction of movement AND the PID controller output
        // hints us to move towards the opposite direction, i.e. due to moving faster than we should
        // or the odometers providing faulty readings, then don't try to spin the motors towards
        // the opposite direction and just halt the motoros. This is because we do not have a
        // reliable way to determine whether we are merely slowing down or accelerating towards
        // the wrong (opposite) direction.
        controlledSpeed = kIdleControlSpeed;
    }
    // Pass the rounded output of the pid as an input to the control
    SimpleCar::setSpeed(lroundf(controlledSpeed));
    // Save the unrounded float output of the PID controller
    mPreviousControlledSpeed = controlledSpeed;
}

float DistanceCar::controlMotorSpeed(const float& previousSpeed,
                                     const float& targetSpeed,
                                     const float& currentSpeed)
{
    float error = targetSpeed - currentSpeed;
    mIntegratedError += error;
    float correction = (mProportional * error) + (mIntegral * mIntegratedError)
                       + (mDerivative * (error - mPreviousError));
    mPreviousError = error;

    return getConstrain(previousSpeed + correction,
                        static_cast<float>(kMinControlSpeed),
                        static_cast<float>(kMaxControlSpeed));
}

float DistanceCar::getSpeed()
{
    return areOdometersAttached() ? (mOdometerLeft.getSpeed() + mOdometerRight.getSpeed()) / 2
                                  : static_cast<float>(kOdometersNotAttachedError);
}

void DistanceCar::disableCruiseControl()
{
    mCruiseControlEnabled = false;
}

void DistanceCar::enableCruiseControl(float proportional,
                                      float integral,
                                      float derivative,
                                      unsigned long frequency)
{
    mCruiseControlEnabled = true;
    mProportional         = proportional;
    mIntegral             = integral;
    mDerivative           = derivative;
    mFrequency            = frequency;
}

bool DistanceCar::areOdometersAttached()
{
    return mOdometerLeft.isAttached() && mOdometerRight.isAttached();
}

bool DistanceCar::areOdometersDirectional()
{
    return mOdometerLeft.providesDirection() && mOdometerRight.providesDirection();
}

void DistanceCar::overrideMotorSpeed(int firstMotorSpeed, int secondMotorSpeed)
{
    if (mCruiseControlEnabled)
    {
        return;
    }

    SimpleCar::overrideMotorSpeed(firstMotorSpeed, secondMotorSpeed);
}
