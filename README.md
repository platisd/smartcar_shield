# CaroloCup2016 physical layer repository
An Arduino library, based on [Smartcar sensors] and the [AndroidCar] libraries, built to control an Autonomous Vehicle made by Team Pegasus, taking part in the Carolo Cup 2016 competition. This library allows the developer to fetch data from the on-board sensors and transmit them, while receiving driving commands to control an ESC and a servo motor.

### Description
The library was designed to control an Autonomous Vehicle, that can follow lanes, park and overtake vehicles, as tasked by the Carolo Cup 2016 competition. The vehicle is controlled by a Linux single board computer (Odroid XU3) that is mounted on top of it and accomplishes its tasks, using image processing and utilizing data from sensors mounted on the car.
Therefore, this library provides an easy to use and simple to understand interface, to control the vehicle's movement and read its sensors data. Despite being used as an in-house system and therefore the developers working on it were already familiriazed with the functions and the various components, the library was designed with the intention of being used by novice users.
The library is dependant on certain hardware sensors and of course the Arduino platform, however, it can be very easily modified or enriched for different setups. In [/examples/CaroloCar] the arduino sketch running on the vehicle can be found. For more examples on the library's usage, please refer to [Example sketches](https://github.com/platisd/AndroidCar/wiki/Example-sketches) page in the repo's wiki.

Moreover, in [/LED_DRIVER] a sketch running on an ATtiny85 microcontroller can be found, which is tasked to control the various LED lights that are mounted on the vehicle, which are indicating the current operation mode of the car (turning, stopping, override, running) as per specifications of the DIT168 course.

Finally, the repository for the application that is running in the single board computer (Odroid) and permits the accomplishment of the various tasks (lane following, parking) can be found [here](https://github.com/platisd/CaroloCup2016/).

### Components
- Electronic Speed Controller (ESC)
- Servo motor (Steering wheel)
- Speed encoder
- Ultrasonic sensors (SRF08)
- Infrared distance sensors (SHARP GP2D120)

### Dependencies
- [Wire library](http://arduino.cc/en/reference/Wire) used for getting data from the SRF08 ultrasonic sensors, via I2C
- [Servo library](http://www.arduino.cc/en/Reference/Servo) used for controlling the ESC and the steering wheel

### Documentation
- Autonomous-Car Arduino Library [Wiki](https://github.com/platisd/AndroidCar/wiki)

[/examples/CaroloCar]:examples/CaroloCar
[/LED_DRIVER]:LED_DRIVER
[Smartcar sensors]:https://github.com/platisd/smartcar_sensors
[AndroidCar]:https://github.com/platisd/AndroidCar

### Project documentation
- [The world's first Android autonomous vehicle](https://platis.solutions/blog/2015/06/29/worlds-first-android-autonomous-vehicle/)
- [Carolo Cup 2016 - Team Pegasus Project](https://github.com/platisd/CaroloCup2016/)

### License
GPLv3
