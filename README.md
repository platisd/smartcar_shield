# Autonomous-Car Arduino Library
An Arduino library, based on [Smartcar sensors], built to control an Autonomous Android Vehicle made by Team Pegasus. This library allows the developer to fetch data from the on-board sensors, transmit them via Bluetooth, while receiving driving commands to control an ESC and a servo motor.

### Description
The library was designed to control an Autonomous Vehicle, that can follow lanes, park and overtake vehicles, as tasked by the [DIT168] course. The vehicle is controlled wirelessly by an Android phone that is mounted on top of it and accomplishes its tasks, using image processing (OpenCV for Android) and utilizing data from sensors mounted on the car.
Therefore, this library provides an easy to use and simple to understand interface, to control the vehicle's movement and read its sensors data. Despite being used as an in-house system and therefore the developers working on it were already familiriazed with the functions and the various components, the library was designed with the intention of being used by novice users.
The library is dependant on certain hardware sensors and of course the Arduino platform, however, it can be very easily modified or enriched for different setups. In [/examples/androidCar] the arduino sketch running on the vehicle can be found.

Moreover, in [/LED_DRIVER] a sketch running on an ATtiny85 microcontroller can be found, which is tasked to control the various LED lights that are mounted on the vehicle, which are indicating the current operation mode of the car (turning, stopping, override, running) as per specifications of the DIT168 course.

### Components
- Electronic Speed Controller (ESC)
- Servo motor (Steering wheel)
- Speed encoder
- Ultrasonic sensors (HC-SR04, SRF05)
- Infrared distance sensors (SHARP GP2D120)
- Gyroscope (L3G4200D)
- 9DOF IMU ([Razor IMU])

### Dependencies
- [Wire library](http://arduino.cc/en/reference/Wire) used for getting data from the gyroscope, via I2C
- [Servo library](http://www.arduino.cc/en/Reference/Servo) used for controlling the ESC and the steering wheel

### Documentation
- Autonomous-Car Arduino Library [Wiki](https://github.com/ylmzcaglar/AndroidCar/wiki)

[/examples/androidCar]:https://github.com/ylmzcaglar/AndroidCar/tree/master/examples/androidCar
[/LED_DRIVER]:https://github.com/ylmzcaglar/AndroidCar/tree/master/LED_DRIVER
[Razor IMU]:https://www.sparkfun.com/products/10736
[Smartcar sensors]:https://github.com/platisd/smartcar_sensors
[DIT168]:http://gul.gu.se/public/courseId/66254/coursePath/46831/ecp/lang-en/publicPage.do

### License
GPLv3