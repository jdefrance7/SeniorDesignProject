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
# Abstract node status information.
# This is the only high-level function that shall be implemented by all nodes.
#
# All UAVCAN nodes that have a node-ID are required to publish this message to its fixed subject periodically.
# Nodes that do not have a node-ID (also known as "anonymous nodes") shall not publish to this subject.
#
# The default subject-ID 32085 is 111110101010101 in binary. The alternating bit pattern at the end
# helps transceiver synchronization (e.g., on CAN-based networks) and on some transports permits
# automatic bit rate detection.
#
# Network-wide health monitoring can be implemented by subscribing to the fixed subject.

uint16 MAX_PUBLICATION_PERIOD = 1 # [second]
# The publication period shall not exceed this limit.
# The period should not change while the node is running.

uint16 OFFLINE_TIMEOUT = 3 # [second]
# If the last message from the node was received more than this amount of time ago, it should be considered offline.

uint32 uptime # [second]
# The uptime seconds counter should never overflow. The counter will reach the upper limit in ~136 years,
# upon which time it should stay at 0xFFFFFFFF until the node is restarted.
# Other nodes may detect that a remote node has restarted when this value leaps backwards.

truncated uint2 health
# Abstract node health information. See constants below.
# Follows:
# https://www.law.cornell.edu/cfr/text/14/23.1322
# https://www.faa.gov/documentLibrary/media/Advisory_Circular/AC_25.1322-1.pdf section 6

uint2 HEALTH_NOMINAL = 0
# The node is functioning properly (nominal).

uint2 HEALTH_ADVISORY = 1
# A critical parameter went out of range or the node encountered a minor failure that does not prevent
# the subsystem from performing any of its real-time functions.

uint2 HEALTH_CAUTION = 2
# The node encountered a major failure and is performing in a degraded mode or outside of its designed limitations.

uint2 HEALTH_WARNING = 3
# The node suffered a fatal malfunction and is unable to perform its intended function.

truncated uint3 mode
# The current operating mode. See constants below.
#
# The mode OFFLINE can be used for informing other network participants that the sending node has ceased its
# activities or about to do so. In this case, other nodes will not have to wait for the OFFLINE_TIMEOUT to
# expire before detecting that the sending node is no longer available.
#
# Reserved values can be used in future revisions of the specification.

uint3 MODE_OPERATIONAL = 0
# Normal operating mode.

uint3 MODE_INITIALIZATION = 1
# Initialization is in progress; this mode is entered immediately after startup.

uint3 MODE_MAINTENANCE = 2
# E.g. calibration, self-test, etc.

uint3 MODE_SOFTWARE_UPDATE = 3
# New software/firmware is being loaded or the bootloader is running.

uint3 MODE_OFFLINE = 7
# The node is no longer available.

truncated uint19 vendor_specific_status_code
# Optional, vendor-specific node status code, e.g. a fault code or a status bitmask.
```

## Task 3 - Send Orientation

Sends a [Record](https://uavcan.org/specification/UAVCAN_Specification_v1.0-alpha.pdf) message onto the CAN bus with orientation data encoded as ASCII text.

```
# Generic human-readable text message for logging and displaying purposes.
# Generally, it should be published at the lowest priority level.

uavcan.time.SynchronizedTimestamp.1.0 timestamp
# Optional timestamp in the network-synchronized time system; zero if undefined.
# The timestamp value conveys the exact moment when the reported event took place.

Severity.1.0 severity

uint8[<=112] text
# Message text.
# Normally, messages should be kept as short as possible, especially those of high severity.
```

