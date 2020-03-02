#ifndef UAVCAN_H
#define UAVCAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "can_config.h"
#include "can.h"
#include <canard.h>
#include <canard_avr.h>
#include <formatting.h>
#include <messaging.h>
#include <uavcan_node.h>

uavcan_node node;
uavcan_node_hardware_version hardware;
uavcan_node_software_version software;

//##############################################################################
// UAVCAN BITRATE //////////////////////////////////////////////////////////////
//##############################################################################

#ifndef CAN_BITRATE

/* Select UAVCAN Bitrate */

// #define CAN_BITRATE BITRATE_10_KBPS	 // not tested

// #define CAN_BITRATE BITRATE_20_KBPS	 // not tested

// #define CAN_BITRATE BITRATE_50_KBPS	 // not tested

// #define CAN_BITRATE BITRATE_100_KBPS // not tested

#define CAN_BITRATE BITRATE_125_KBPS

// #define CAN_BITRATE BITRATE_250_KBPS // not tested

// #define CAN_BITRATE BITRATE_500_KBPS // not tested

// #define CAN_BITRATE BITRATE_1_MBPS   // not tested

#endif

//##############################################################################
// UAVCAN RESPONSES ////////////////////////////////////////////////////////////
//##############################################################################

#ifndef SUPPORTED_UAVCAN_RESPONSES
#define SUPPORTED_UAVCAN_RESPONSES

/* Select Supported UAVCAN Responses Addressed to this Node  */

#define ACCEPT_RESPONSE_NONE

// Add other response flags here (must also write support code in callback/handler)

#endif

//##############################################################################
// UAVCAN REQUESTS /////////////////////////////////////////////////////////////
//##############################################################################

#ifndef SUPPORTED_UAVCAN_REQUESTS
#define SUPPORTED_UAVCAN_REQUESTS

/* Select Supported UAVCAN Requests Addressed to this Node  */

#define ACCEPT_REQUEST_NONE

// #define ACCEPT_REQUEST_NODE_INFO

// #define ACCEPT_REQUEST_DATA_TYPE_INFO

// Add other request flags here (must also write support code in callback/handler)

#endif

//##############################################################################
// UAVCAN BROADCASTS ///////////////////////////////////////////////////////////
//##############################################################################

#ifndef SUPPORTED_UAVCAN_BROADCASTS
#define SUPPORTED_UAVCAN_BROADCASTS

/* Select Supported UAVCAN Broadcasts Addressed to this Node  */

#define ACCEPT_BROADCAST_NONE

// Add other broadcast flags here (must also write support code in callback/handler)

#endif

//##############################################################################
// CANARD OBJECTS //////////////////////////////////////////////////////////////
//##############################################################################

// Library instance
CanardInstance g_canard;
uint8_t g_canard_memory_pool[1024];

//##############################################################################
// USER FUNCTIONS //////////////////////////////////////////////////////////////
//##############################################################################

bool init_uavcan();
int16_t uavcan_stats();
int16_t cleanup_uavcan(uint64_t timestamp_usec);

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

#ifdef __cplusplus
}
#endif

#endif // UAVCAN_H
