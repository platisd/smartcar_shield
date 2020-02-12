/**
 * \interface HeadingSensor
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
     *
     * **Example:**
     * \code
     * sensor.getHeading();
     * \endcode
     */
    virtual int getHeading() = 0;

    /**
     * Updates the sensor's readings. This method **must** be be able to be executed
     * as often as possible.
     *
     * **Example:**
     * \code
     * void loop() {
     *   sensor.update();
     *   // More code
     * }
     * \endcode
     */
    virtual void update() = 0;
};
