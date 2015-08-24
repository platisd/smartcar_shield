#Sketch set up

This sketch utilizes the Autonomous Car library and the [Netstrings library](https://github.com/platisd/Netstrings) in order to send data fetched from the on board sensors to an Android phone, while receiving driving instructions from it. The current system schematic can be found in the [/res](res) folder, while various pin definitions are included in the [CarVariables.h](CarVariables.h)

The Android application that interpets the data sent via this sketch and sends the driving instructions, can be found in the [Android-Car-duino](https://github.com/Petroula/Android-Car-duino/) repository.
