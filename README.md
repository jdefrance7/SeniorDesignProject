# Senior Design Project
Electrical Engineering Senior Design Project at the University of Minnesota - Twin Cities, 2020

The purpose of this project is to read IMU sensor data from individual wing segments and send it to the Pixhawk flight controller over CAN for the purpose of obtaining wing segment orientation data to be used for control calculations and diagnotstics.

## Getting Started
This project was created using the Arduino IDE for the AST-CAN485 development board. 

[Arduino IDE](https://www.arduino.cc/en/main/software)

[AST-CAN485](https://www.sparkfun.com/products/14483)

### Dependencies
A specific compiler set and multiple third-party libraries were used and adapted to fit this project's needs.

|Compiler               | IMU Libraries             | CAN Libraries                 |
|-----------------------|---------------------------|-------------------------------|
|[AVR-GCC][1]           |[Adafruit_BNO055][3]       |[libcanard][9]                 |
|[AVR-G++][2]           |[Adafruit_LSM9DS1][4]      |[Universal Can Library][10]    |
|                       |[Adafruit_FXOS8700][5]     |                               |
|                       |[Adafruit_FXAS21002C][6]   |                               |
|                       |[Adafruit_AHRS][7]         |                               |
|                       |[Adafruit_Sensor][8]       |                               |


[1]: (https://gcc.gnu.org/wiki/avr-gcc)
[2]: (https://linux.die.net/man/1/avr-g++)
[3]: (https://github.com/adafruit/Adafruit_BNO055)
[4]: (https://github.com/adafruit/Adafruit_LSM9DS1)
[5]: (https://github.com/adafruit/Adafruit_FXOS8700)
[6]: (https://github.com/adafruit/Adafruit_FXAS21002C)
[7]: (https://github.com/adafruit/Adafruit_AHRS)
[8]: (https://github.com/adafruit/Adafruit_Sensor)
[9]: (https://github.com/UAVCAN/libcanard)
[10]: (https://github.com/rennerm/avr-can-lib/tree/9c6bc9118de66d6edaf1b8539e2b9717ba26d123#universelle-can-blibiothek-avr-can-lib)

## Implementation
For our purposes, the `libcanard` library was split into three sub-libraries:

1. `canard` - contains the basic C implementation of UAVCAN

    `canard.h`

    `canard.c`

    `canard_internals.h`

2. `can` - contains the CAN driver for the AT90CAN128 microcontroller

    `can.h`

    `libcan.a`

    `can_config.h`

3. `canard_avr` - used to bridge 'canard' and 'can'

    `canard_avr.h`

    `canard_avr.c`
    
See [libcanard](https://github.com/UAVCAN/libcanard) for more details on how the UAVCAN stack is implemented.

## Authors
Joe DeFrance, Alex Mootz, Andrew Schweyen, George Sahyoun, Luke Timper, Nathan Raab

## Acknowledgements
We would like to thank Professor Nikolaos Papanikolopoulos, Travis Hanson, and all of the team members in the Solar UAV lab at the University of Minnesota - Twin Cities for allowing us to work on this amazing project with them.

We would also like to acknowledge Professor Paul Imbertson for his time and effort as the Senior Design Faculty Mentor.
