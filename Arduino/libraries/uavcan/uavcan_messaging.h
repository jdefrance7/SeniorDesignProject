#ifndef UAVCAN_MESSAGING_H
#define UAVCAN_MESSAGING_H

//##############################################################################
// DESCRIPTION /////////////////////////////////////////////////////////////////
//##############################################################################

// Provides function wrappers for interfacing with the libcanard drivers

//##############################################################################
// INCLUDES ////////////////////////////////////////////////////////////////////
//##############################################################################

// Libcanard source, CAN drivers, UAVCAN interface
#include "libcanard.h"

//##############################################################################
// FUNCTIONS ///////////////////////////////////////////////////////////////////
//##############################################################################

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------

/* Reads (and pops) a single message frame from the rx queue
    Returns error value
*/
int16_t read_message(
  CanardCANFrame* rx_frame,
  const uint64_t timestamp
);

/* Pushes a message frame onto tx queue
    Returns error value or number of frames enqueued
    Wrapper for canardBroadcast()
*/
int16_t push_message(
  uint64_t data_type_signature,   ///< See above
  uint16_t data_type_id,          ///< Refer to the specification
  uint8_t* inout_transfer_id,     ///< Pointer to a persistent variable containing the transfer ID
  uint8_t priority,               ///< Refer to definitions CANARD_TRANSFER_PRIORITY_*
  const void* payload,            ///< Transfer payload
  uint16_t payload_len            ///< Length of the above, in bytes
);

/* Sends (and pops) a single message frame from the tx queue
*/
int16_t send_message();

/* Send (and pop) all available message frames from tx queue
*/
int16_t flush_messages();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // UAVCAN_MESSAGING_H
