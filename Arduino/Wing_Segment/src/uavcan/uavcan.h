#ifndef UAVCAN_H
#define UAVCAN_H

//##############################################################################
// INCLUDES ////////////////////////////////////////////////////////////////////
//##############################################################################

#include "libcanard.h"
#include "uavcan_node.h"
#include "uavcan_formatting.h"
#include "uavcan_messaging.h"

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

//##############################################################################
// UAVCAN NODE /////////////////////////////////////////////////////////////////
//##############################################################################

uavcan_node node;
uavcan_node_hardware_version hardware;
uavcan_node_software_version software;

//##############################################################################
// USER FUNCTIONS //////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

bool init_uavcan();
int16_t uavcan_stats();
int16_t cleanup_uavcan(uint64_t timestamp_usec);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//##############################################################################
// CALLBACK & HANDLER FUNCTIONS ////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // UAVCAN_H
