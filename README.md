# Senior Design Project

Electrical Engineering Senior Design Project at the University of Minnesota - Twin Cities, Spring 2020

The purpose of this project is to obtain real-time orientation data of multiple UAV wing segments.

## Authors

Joe DeFrance (defra022@umn.edu)

Alex Mootz (mootz017@umn.edu)

Andrew Schweyen (schwe596@umn.edu)

Nathan Raab (raabx028@umn.edu)

George Sahyoun (sahyo002@umn.edu)

Luke Timper (timpe005@umn.edu)

## Getting Started

This project uses [Git](https://git-scm.com/about) for documentation control. Please [download Git](https://git-scm.com/downloads) if it is not installed already.

### Installation

1. Open a terminal.

2. Navigate to the desired working directory.

3. Type `git clone` into the terminal and then copy the link provided by clicking on the green `Clone or Download` button on the project homepage.

4. Press `Enter` and wait for the download to complete.

### Implementation

Please read through the sub-folders of this project to understand the intended implementation of its various components.

#### AST-CAN485

Uses the AST-CAN485 development board and Adafruit BNO055 sensors to obtain and broadcast orientation data via CAN to a PX4 flight controller.

#### PX4

Custom PX4 Firmware stack to read the incomming orientation data and publish it to an internal messaging system.

## Acknowledgements

We would like to thank Professor Nikolaos Papanikolopoulos, Travis Hanson, and all of the team members in the Solar UAV Lab at the University of Minnesota - Twin Cities for allowing us to work on this amazing project with them.

We would also like to thank Professor Paul Imbertson for his time and effort as our Senior Design Advisor.
