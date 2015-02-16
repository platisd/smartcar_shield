# Smartcar Sensors
A library that allows the developer to easily utilize various sensors such as the HC-SR04 (ultra sound), the Sharp GP2Y0A02 (infrared) and others. Written by Dimitris Platis.

### What
This library was initially designed for a specific hardware platform (an Arduino based remote controlled car), the "Smartcar" and is developed for educational purposes within the context of an experiment on API evaluation, by Dimitris Platis. It is supposed to serve as a supplement to the [Smartcar Core](https://github.com/platisd/smartcar_core/) library. However, this library is not dependent on the Smartcar platform or the Smartcar Core library, can be used stand alone and deployed in various settings. During the development of the API, the assumption that the API users are inexperienced and opportunistic was made.
This is justified by the fact that the students who will use it, have not programmed in an Arduino or another embedded platform before, are not expert programmers and are using the Smartcar platform just because they have to pass their university course.
Therefore, some library components might not be accessible to them. During the usage of the library, the validity of those choices will be verified and if the assumptions are deemed void, components that used to be private could be made public, in a transparent and non-disruptive manner.

### How
Students of the [DIT524] course, of University of Gothenburg will be tasked to develop a system composed of at least an Android Application and an Arduino sketch controlling the car.
- The Android application that shall remotely control the Smartcar, get from and send data to it and initiate automated tasks.
- The Arduino sketch will utilize the Smartcar sensor library in order to easily control and read the sensors of the car. This will help the students to implement the various functionalities needed in order to satisfy their business cases.

The students will provide constant feedback on the API, which will aid in the extraction of various findings and conclusions on the subject of API evaluation.

### Dependencies
(None)

### Documentation
- [Wiki]

[Wiki]:https://github.com/platisd/smartcar_sensors/wiki
[DIT524]:http://gul.gu.se/public/courseId/66245/coursePath/46825/ecp/lang-sv/publicPage.do

### License
GPLv3