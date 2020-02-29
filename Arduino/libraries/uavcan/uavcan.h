#ifndef UAVCAN_H
#define UAVCAN_H

#include <canard.h>
#include <canard_avr.h>
#include <formatting.h>
#include <messaging.h>

//##############################################################################
// UAVCAN CONFIGS //////////////////////////////////////////////////////////////
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

/* Select Supported UAVCAN Responses Addressed to this Node */

// #define ACCEPT_RESPONSE_NONE

// Add other response flags here (must also write support code in callback/handler)

/* Select Supported UAVCAN Requests Addressed to this Node  */

// #define ACCEPT_REQUEST_NONE
// #define ACCEPT_REQUEST_NODE_INFO
// #define ACCEPT_REQUEST_DATA_TYPE_INFO

// Add other request flags here (must also write support code in callback/handler)

/* Select Supported UAVCAN Broadcasts Addressed to this Node  */

// #define ACCEPT_BROADCAST_NONE

// Add other broadcast flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN OBJECTS //////////////////////////////////////////////////////////////
//##############################################################################

// Library instance
static CanardInstance g_canard;
static uint8_t g_canard_memory_pool[1024];

// Node unique id
static uint8_t g_local_node_id = 0;

// Node status variables
static uint8_t g_node_health  = UAVCAN_NODE_HEALTH_OK;
static uint8_t g_node_mode    = UAVCAN_NODE_MODE_INITIALIZATION;
static uint64_t g_node_uptime = 0;
static uint8_t g_node_temp    = UAVCAN_NODE_TEMP_OK;

// Node hardware version variables
static uint8_t g_hardware_version_major = 1;
static uint8_t g_hardware_version_minor = 0;

// Node software version variables
static uint8_t  g_software_version_major  = 1;
static uint8_t  g_software_version_minor  = 0;
static uint8_t  g_optional_field_flags    = 1;
static uint32_t g_vcs_commit              = 0;
static uint64_t g_image_crc               = 0;

// Node name
static uint8_t  g_node_name = "wing_segment_n";

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
