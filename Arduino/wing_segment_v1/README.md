# Wing Segmet (UAVCAN v1)

Implements a UAVCAN node for the purpose of sending IMU data of wing segment orientation onto a CAN bus.

Uses a sequential task structure with separate execution periods for each task.

## Task 0 - Setup

Initializes all modules according to configuration values set in the source file. Called once upon powering the module.

## Task 1 - Update IMU

Updates the IMU processing filters for the LSM9DS1 and NXP_FXOS_FXAS, ignored by the BNO055. Must be done at a freqency of `FILTER_UPDATE_RATE_HZ` defined by the IMU used.

## Task 2 - Send Heartbeat

Sends a [Heartbeat](https://uavcan.org/specification/UAVCAN_Specification_v1.0-alpha.pdf) message onto the CAN bus. Required by every UAVCAN node at least once every second.

```
#
# Abstract node status information.
#

uint16 MAX_PUBLICATION_PERIOD = 1 # [second]

uint16 OFFLINE_TIMEOUT = 3 # [second]

#
# The uptime seconds counter should never overflow.
#
uint32 uptime # [second]

#
# Abstract node health information. 
#
truncated uint2 health

uint2 HEALTH_NOMINAL = 0
uint2 HEALTH_ADVISORY = 1
uint2 HEALTH_CAUTION = 2
uint2 HEALTH_WARNING = 3

#
# The current operating mode.
#
truncated uint3 mode

uint3 MODE_OPERATIONAL = 0
uint3 MODE_INITIALIZATION = 1
uint3 MODE_MAINTENANCE = 2
uint3 MODE_SOFTWARE_UPDATE = 3
uint3 MODE_OFFLINE = 7

#
# Optional, vendor-specific node status code.
#
truncated uint19 vendor_specific_status_code

```

## Task 3 - Send Orientation

Sends a [Record](https://uavcan.org/specification/UAVCAN_Specification_v1.0-alpha.pdf) message onto the CAN bus with orientation data encoded as ASCII text.

```
#
# Generic human-readable text message for logging and displaying purposes.
#

#
# Optional timestamp in the network-synchronized time system; zero if undefined.
#
uavcan.time.SynchronizedTimestamp.1.0 timestamp

Severity.1.0 severity

#
# Message text.
#
uint8[<=112] text
```

