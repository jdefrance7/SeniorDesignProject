/**
 *  @file wing_segment_config_1.h
 *
 *  Configuration file for wing segment orientation system with identifier 1.
 *
 *  @author Joe DeFrance
 */

#ifndef WING_SEGMENT_CONFIG_H
#define WING_SEGMENT_CONFIG_H

//******************************************************************************
// Serial Debugging

#define SERIAL_DEBUG // comment out for production, uncomment for debug information
#define SERIAL_BAUDRATE 115200

//******************************************************************************
// Select Angular Comand Orientation Format

#define SEND_ORIENTATION_AS_QUATERNION
//#define SEND_ORIENTATION_AS_EULER_VECTOR

//******************************************************************************
// Select IMU

#define IMU_BNO055
//#define IMU_LSM9DS1
//#define IMU_NXP_FXOS_FXAS

//******************************************************************************
// CAN Bitrate

#define CAN_BITRATE 1000000            // AST_CAN

//******************************************************************************
// CAN Module Timeout (ms)

#define TRANSMIT_TIMEOUT 5

//******************************************************************************
// CAN Inter-Frame Delay (ms)

#define TRANSMIT_DELAY 10

//******************************************************************************
// Dynamic Task Periods (ms)

#define SEND_NODE_STATUS_PERIOD_MS  (MAX_BROADCASTING_PERIOD_MS / 2)
#define SEND_ORIENTATION_PERIOD_MS  (100)

//******************************************************************************
// Static Task Periods (ms)

#define UPDATE_IMU_PERIOD_MS        (1000 / FILTER_UPDATE_RATE_HZ)
#define CLEANUP_UAVCAN_PERIOD_MS    (CANARD_RECOMMENDED_STALE_TRANSFER_CLEANUP_INTERVAL_USEC / 1000)

//******************************************************************************
// Wing Segment Number

#define WING_SEGMENT  1

//******************************************************************************
// Node Information

#define NODE_ID   21
#define NODE_NAME "1.winglet.uavcan"

//******************************************************************************
// Node Hardware Information

#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

#define HARDWARE_UNIQUE_ID    "123456789ABCDEF"
#define HARDWARE_CERTIFICATE  "Certificate"

//******************************************************************************
// Node Software Information

#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0

#define SOFTWARE_OPTIONAL_FIELD_FLAGS 0
#define SOFTWARE_VCS_COMMIT           0
#define SOFTWARE_IMAGE_CRC            0

//******************************************************************************
// LED Toggle Rate (ms)

#define LED_DEFAULT 400
#define LED_FAST    250
#define LED_MEDIUM  500
#define LED_SLOW    750

//******************************************************************************

#endif // WING_SEGMENT_CONFIG_H
