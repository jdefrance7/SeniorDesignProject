# Senior Design Project
Electrical Engineering Senior Design Project at the University of Minnesota - Twin Cities, 2020

The purpose of this project is to read IMU sensor data from individual wing segments and send it to the Pixhawk flight controller over CAN for the purpose of obtaining wing segment orientation data to be used for control calculations and diagnotstics.

## Getting Started
This project was created using the Arduino IDE for the AST-CAN485 development board. 

[Arduino IDE](https://www.arduino.cc/en/main/software)

[AST-CAN485](https://www.sparkfun.com/products/14483)

### Installing

1. Download the Arduino IDE.
2. Replace the created `Arduino` folder with the one from this repository.

   * Alternatively copy the `Arduino\libraries` folder contents to your local `Arduino\libraries` and move any sketch folders to the `Arduino` folder.
3. Use the Boards Manager to add the AST-CAN485 board to the Arduino IDE.

   * Follow the steps in the [AST-CAN485 Hookup Guide](https://learn.sparkfun.com/tutorials/ast-can485-hookup-guide?_ga=2.6567157.631132834.1583434944-271346267.1574810854) section 'Board Installation Using the Boards Manager'.

### Compiling

1. Select the AST-CAN485 board from the `Tools->Board:` menu.
2. Either click the check mark button in the upper left hand corner of the IDE or select `Sketch->Verify/Compile`
3. Read diagnostic information from the Arduino IDE for any warnings or errors.

### Uploading

1. Connect the AST-CAN485 to a computer through a [FTDI Adaptor](https://www.sparkfun.com/products/9716?_ga=2.13897968.631132834.1583434944-271346267.1574810854)
2. Select the proper port from the `Tools->Port:` menu.
3. Select "PROGRAMMER" from the `Tools-Programmer:` menu.
4. select `Sketch->Upload Using Programmer` from the toolbar.

## Notes

### Dependencies
A specific compiler set and multiple third-party libraries were used and adapted to fit this project's needs.

Compiler Set
* [AVR-GCC](https://gcc.gnu.org/wiki/avr-gcc)
* [AVR-G++](https://linux.die.net/man/1/avr-g++)

IMU Libraries
* [Adafruit_BNO055](https://github.com/adafruit/Adafruit_BNO055)
* [Adafruit_LSM9DS1](https://github.com/adafruit/Adafruit_LSM9DS1)
* [Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700)
* [Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C)
* [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

Power Sensor Library
* [Adafruit_INA219](https://github.com/adafruit/Adafruit_INA219)

CAN Libraries
* [libcanard](https://github.com/UAVCAN/libcanard)
* [Universal Can Library](https://github.com/rennerm/avr-can-lib/tree/9c6bc9118de66d6edaf1b8539e2b9717ba26d123#universelle-can-blibiothek-avr-can-lib)

### UAVCAN
For our purposes, the `libcanard` library was split into three sub-libraries:

1. `<canard>` - contains the basic C implementation of UAVCAN

    `canard.h`

    `canard.c`

    `canard_internals.h`

2. `<can>` - contains the CAN driver for the AT90CAN128 microcontroller

    `can.h`

    `libcan.a`

    `can_config.h`

3. `<canard_avr>` - used to bridge `<canard>` and `<can>`

    `canard_avr.h`

    `canard_avr.c`
    
See [libcanard](https://github.com/UAVCAN/libcanard) for more details on how UAVCAN is implemented by the `libcanard` stack and its drivers.

## Authors
Joe DeFrance, Alex Mootz, Andrew Schweyen, George Sahyoun, Luke Timper, Nathan Raab

## Acknowledgements
We would like to thank Professor Nikolaos Papanikolopoulos, Travis Hanson, and all of the team members in the Solar UAV lab at the University of Minnesota - Twin Cities for allowing us to work on this amazing project with them.

We would also like to acknowledge Professor Paul Imbertson for his time and effort as the Senior Design Faculty Mentor.
