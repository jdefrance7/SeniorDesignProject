#ifndef UAVCAN_CONFIG_H
#define UAVCAN_CONFIG_H

#include <canard_avr.h>

//##############################################################################
// SELECT UAVCAN BITRATE ///////////////////////////////////////////////////////
//##############################################################################

/* Select UAVCAN Bitrate */

// #define CAN_BITRATE BITRATE_10_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_20_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_50_KBPS	 // not tested
// #define CAN_BITRATE BITRATE_100_KBPS // not tested
#define CAN_BITRATE BITRATE_125_KBPS
// #define CAN_BITRATE BITRATE_250_KBPS // not tested
// #define CAN_BITRATE BITRATE_500_KBPS // not tested
// #define CAN_BITRATE BITRATE_1_MBPS   // not tested

//##############################################################################
// SELECT SUPPORTED UAVCAN RESPONSES ///////////////////////////////////////////
//##############################################################################

#define SUPPORTED_UAVCAN_RESPONSES

/* Select Supported UAVCAN Responses Addressed to this Node  */

#define ACCEPT_RESPONSE_NONE

// Add other response flags here (must also write support code in callback/handler)

//##############################################################################
// SELECT SUPPORTED UAVCAN REQUESTS ////////////////////////////////////////////
//##############################################################################

#define SUPPORTED_UAVCAN_REQUESTS

/* Select Supported UAVCAN Requests Addressed to this Node  */

// #define ACCEPT_REQUEST_NONE

#define ACCEPT_REQUEST_NODE_INFO

// #define ACCEPT_REQUEST_DATA_TYPE_INFO

// Add other request flags here (must also write support code in callback/handler)

//##############################################################################
// SELECT SUPPORTED UAVCAN BROADCASTS //////////////////////////////////////////
//##############################################################################

#define SUPPORTED_UAVCAN_BROADCASTS

/* Select Supported UAVCAN Broadcasts Addressed to this Node  */

#define ACCEPT_BROADCAST_NONE

// Add other broadcast flags here (must also write support code in callback/handler)

//##############################################################################
// UAVCAN LIBRARY //////////////////////////////////////////////////////////////
//##############################################################################

/* UAVCAN Library */

#include <uavcan.h>

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // UAVCAN_CONFIG_H
