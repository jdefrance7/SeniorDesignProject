#ifndef MESSAGING_H
#define MESSAGING_H

#include <canard_avr.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Reads (and pops) a single message frame from the rx queue
    Stores read message frame in rx_frame
    Returns error value
*/
int16_t read_message(
  CanardInstance* ins,
  CanardCANFrame* rx_frame
);

/* Pushes a message frame onto tx queue
    Returns error value or number of frames enqueued
    Wrapper for canardBroadcast()
*/
int16_t push_message(
  CanardInstance* ins,            ///< Library instance
  uint64_t data_type_signature,   ///< See above
  uint16_t data_type_id,          ///< Refer to the specification
  uint8_t* inout_transfer_id,     ///< Pointer to a persistent variable containing the transfer ID
  uint8_t priority,               ///< Refer to definitions CANARD_TRANSFER_PRIORITY_*
  const void* payload,            ///< Transfer payload
  uint16_t payload_len            ///< Length of the above, in bytes
);

/* Sends (and pops) a single message frame from the tx queue
    Stores sent message frame in tx_frame
    Returns error value
*/
int16_t send_message(
  CanardInstance* ins,
  CanardCANFrame* tx_frame
);

/* Send (and pop) all available message frames from tx queue
    Returns error value
*/
int16_t flush_messages(
  CanardInstance* ins,
  CanardCANFrame* tx_frame
);

#ifdef __cplusplus
}
#endif

#endif // MESSAGING_H
