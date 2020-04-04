# Wing Segmet (UAVCAN v0)

Implements a UAVCAN node for the purpose of sending IMU data of wing segment orientation onto a CAN bus.

Uses a sequential task structure with separate execution periods for each task.

## Task 0 - Setup

Initializes all modules according to configuration values set in the source file. Called once upon powering the module.

## Task 1 - Update IMU

Updates the IMU processing filters for the LSM9DS1 and NXP_FXOS_FXAS, ignored by the BNO055. Must be done at a freqency of `FILTER_UPDATE_RATE_HZ` defined by the IMU used.

## Task 2 - Send Node Status

Sends a Node Status message onto the CAN bus. Required by every UAVCAN node at least once every second.

## Task 3 - Send Orientation

Sends orientation data onto the CAN bus in two ways:

1. Sends a Log Message message onto the CAN bus with orientation data encoded as ASCII text.

2. Sends a Camera Gimbal Status message onto the CAN bus with orientation data included as an internal variable.

## Task 4 - Check Memory Pool

Checks the memory usage statistics of the Canard memory pool. Mainly for debugging.
