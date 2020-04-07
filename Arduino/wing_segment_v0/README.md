# Wing Segmet (UAVCAN v0)

Implements a UAVCAN node for the purpose of sending IMU data of wing segment orientation onto a CAN bus.

Uses a sequential task structure with separate execution periods for each task.

## Task 0 - Setup

Initializes all modules according to configuration values set in the source file. Called once upon powering the module.

## Task 1 - Update IMU

Updates the IMU processing filters for the LSM9DS1 and NXP_FXOS_FXAS, ignored by the BNO055. Must be done at a freqency of `FILTER_UPDATE_RATE_HZ` defined by the IMU used.

## Task 2 - Send Node Status

Sends a [Node Status](https://legacy.uavcan.org/Specification/7._List_of_standard_data_types/) message onto the CAN bus. Required by every UAVCAN node at least once every second.

```
#
# Abstract node status information.
#
# Any UAVCAN node is required to publish this message periodically.
#

#
# Publication period may vary within these limits.
# It is NOT recommended to change it at run time.
#
uint16 MAX_BROADCASTING_PERIOD_MS = 1000
uint16 MIN_BROADCASTING_PERIOD_MS = 2

#
# If a node fails to publish this message in this amount of time, it should be considered offline.
#
uint16 OFFLINE_TIMEOUT_MS = 3000

#
# Uptime counter should never overflow.
# Other nodes may detect that a remote node has restarted when this value goes backwards.
#
uint32 uptime_sec

#
# Abstract node health.
#
uint2 HEALTH_OK         = 0     # The node is functioning properly.
uint2 HEALTH_WARNING    = 1     # A critical parameter went out of range or the node encountered a minor failure.
uint2 HEALTH_ERROR      = 2     # The node encountered a major failure.
uint2 HEALTH_CRITICAL   = 3     # The node suffered a fatal malfunction.
uint2 health

#
# Current mode.
#
# Mode OFFLINE can be actually reported by the node to explicitly inform other network
# participants that the sending node is about to shutdown. In this case other nodes will not
# have to wait OFFLINE_TIMEOUT_MS before they detect that the node is no longer available.
#
# Reserved values can be used in future revisions of the specification.
#
uint3 MODE_OPERATIONAL      = 0         # Node is performing its main functions.
uint3 MODE_INITIALIZATION   = 1         # Node is initializing; this mode is entered immediately after startup.
uint3 MODE_MAINTENANCE      = 2         # Node is under maintenance.
uint3 MODE_SOFTWARE_UPDATE  = 3         # Node is in the process of updating its software.
uint3 MODE_OFFLINE          = 7         # Node is no longer available.
uint3 mode

#
# Not used currently, keep zero when publishing, ignore when receiving.
#
uint3 sub_mode

#
# Optional, vendor-specific node status code, e.g. a fault code or a status bitmask.
#
uint16 vendor_specific_status_code
```

## Task 3 - Send Orientation

Sends orientation data onto the CAN bus in two ways:

1. Sends a [Log Message](https://legacy.uavcan.org/Specification/7._List_of_standard_data_types/) message onto the CAN bus with orientation data encoded as ASCII text.

    ```
    #
    # Generic log message.
    # All items are byte aligned.
    #

    LogLevel level
    uint8[<=31] source
    uint8[<=90] text
    ```

2. Sends a [Camera Gimbal Status](https://legacy.uavcan.org/Specification/7._List_of_standard_data_types/) message onto the CAN bus with orientation data included as an internal variable.

    ```
    #
    # Generic gimbal status.
    #

    uint8 gimbal_id

    Mode mode

    #
    # Camera axis orientation in body frame (not in fixed frame).
    # Please refer to the UAVCAN coordinate frame conventions.
    #
    float16[4] camera_orientation_in_body_frame_xyzw
    float16[<=9] camera_orientation_in_body_frame_covariance   # +inf for non-existent axes
    ```

## Task 4 - Check Memory Pool

Checks the memory usage statistics of the Canard memory pool. Debugging.
