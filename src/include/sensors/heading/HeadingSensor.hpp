/**
 * A heading sensor is an interface that can represent all sensors which can
 * provide the vehicle's heading.
 */
#pragma once

class HeadingSensor
{
public:
    virtual ~HeadingSensor() = default;

    /**
     * Returns the current heading of the vehicle
     * @return The current heading of the vehicle in the range of [0-360)
     */
    virtual unsigned int getHeading() = 0;

    /**
     * Updates the sensor's readings (if necessary)
     */
    virtual void update() = 0;
};
