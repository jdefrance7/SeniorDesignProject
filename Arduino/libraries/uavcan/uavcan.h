#ifndef UAVCAN_H
#define UAVCAN_H

#include <canard.h>
#include <canard_avr.h>
#include <formatting.h>
#include <messaging.h>

#include "uavcan_node.h"

//##############################################################################
// UAVCAN BITRATE //////////////////////////////////////////////////////////////
//##############################################################################

/* Select UAVCAN Bitrate */

// #define CAN_BITRATE BITRATE_10_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_20_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_50_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_100_KBPS // not tested
// #define CAN_BITRATE BITRATE_125_KBPS
// #define CAN_BITRATE BITRATE_250_KBPS // not tested
// #define CAN_BITRATE BITRATE_500_KBPS // not tested
// #define CAN_BITRATE BITRATE_1_MBPS   // not tested

//##############################################################################
// UAVCAN RESPONSES ////////////////////////////////////////////////////////////
//##############################################################################

/* Select Supported UAVCAN Responses Addressed to this Node  */

// #define ACCEPT_RESPONSE_NONE

// Add other response flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN REQUESTS /////////////////////////////////////////////////////////////
//##############################################################################

/* Select Supported UAVCAN Requests Addressed to this Node  */

// #define ACCEPT_REQUEST_NONE

// #define ACCEPT_REQUEST_NODE_INFO
// #define ACCEPT_REQUEST_DATA_TYPE_INFO

// Add other request flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN BROADCASTS ///////////////////////////////////////////////////////////
//##############################################################################

/* Select Supported UAVCAN Broadcasts Addressed to this Node  */

// #define ACCEPT_BROADCAST_NONE

// Add other broadcast flags here (must also write support code in callback/handler)

//##############################################################################
// CANARD OBJECTS //////////////////////////////////////////////////////////////
//##############################################################################

// Library instance
CanardInstance g_canard;
uint8_t g_canard_memory_pool[1024];

//##############################################################################
// UAVCAN NODE /////////////////////////////////////////////////////////////////
//##############################################################################

uint8_t node_name = "n.segment.wing";

// Node variables
node.local_id = 1;
node.health   = UAVCAN_NODE_HEALTH_OK;
node.mode     = UAVCAN_NODE_MODE_INITIALIZATION;
node.uptime   = 0;
node.temp     = UAVCAN_NODE_TEMP_OK;
node.name     = &node_name;

//##############################################################################
// NODE HARDWARE INFO //////////////////////////////////////////////////////////
//##############################################################################

uint8_t node_unique_id[16] = {0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00};

uint8_t node_certificate = "Certificate";

// Hardware version variables
hardware.major        = 1;
hardware.minor        = 0;
hardware.unique_id    = &node_unique_id;
hardware.certificate  = &node_certificate;

//##############################################################################
// NODE SOFTWARE INFO //////////////////////////////////////////////////////////
//##############################################################################

// Software version variables
software.major        = 1;
software.minor        = 0;
software.field_flags  = 1;
software.vcs_commit   = GIT_HASH;
software.image_crc    = 0;

//##############################################################################
// USER FUNCTIONS //////////////////////////////////////////////////////////////
//##############################################################################

bool init_uavcan();
uint16_t uavcan_stats();
bool cleanup_uavcan(uint64_t timestamp_usec);

//##############################################################################
// CALLBACK & HANDLER FUNCTIONS ////////////////////////////////////////////////
//##############################################################################

void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id);

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // UAVCAN_H
