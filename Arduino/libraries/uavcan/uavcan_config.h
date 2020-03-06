#ifndef UAVCAN_CONFIG_H
#define UAVCAN_CONFIG_H

#include <can.h>

//##############################################################################
// UAVCAN CONFIG ///////////////////////////////////////////////////////////////
//##############################################################################

/* Select UAVCAN Bitrate */

// #define CAN_BITRATE BITRATE_10_KBPS	   // not tested
// #define CAN_BITRATE BITRATE_20_KBPS	   // not tested
// #define CAN_BITRATE BITRATE_50_KBPS	   // not tested
// #define CAN_BITRATE BITRATE_100_KBPS    // not tested
#define CAN_BITRATE BITRATE_125_KBPS
// #define CAN_BITRATE BITRATE_250_KBPS    // not tested
// #define CAN_BITRATE BITRATE_500_KBPS    // not tested
// #define CAN_BITRATE BITRATE_1_MBPS      // not tested

//------------------------------------------------------------------------------

/* Setup UAVCAN Node Info */

#define LOCAL_NODE_ID 1

static const unsigned char LOCAL_NODE_NAME[] = "n.segment.wing";

//------------------------------------------------------------------------------

/* Setup UAVCAN Node Hardware Info */

#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

static const uint8_t NODE_UNIQUE_ID[16] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

static const unsigned char NODE_CERTIFICATE[] = "Certificate";

//------------------------------------------------------------------------------

/* Setup UAVCAN Node Software Info */

#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0

#define SOFTWARE_VERSION_FIELD_FLAGS  0
#define SOFTWARE_VERSION_VCS_COMMIT   0
#define SOFTWARE_VERSION_IMAGE_CRC    0

//------------------------------------------------------------------------------

#endif // UAVCAN_CONFIG_H
