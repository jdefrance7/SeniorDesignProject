#include "uavcan_receiving_v0.h"

// Using Canard_AVR
#if defined(CANARD_AVR_H)
int8_t read_message_frame(CanardInstance canard, CanardCANFrame* rx_frame, const uint64_t timestamp)
{
  // Receive CAN frame (Failure = -1, Success = 1, Timeout = 0)
  const int16_t rx_res = canardAVRReceive(rx_frame);
  if (rx_res < 0)
  {
    // Error receiving frame
    return -1;
  }
  else if (rx_res > 0)
  {
    return canardHandleRxFrame(&canard, rx_frame, timestamp);
  }
  else
  {
    // No frames available
    return 1;
  }
}
#endif

// Using ASTCanLib
#if defined(_CAN_LIB_H_)
int8_t read_message_frame(CanardInstance canard, CanardCANFrame* rx_frame, const uint64_t timestamp)
{
  // TODO: write this

  return 1;
}
#endif

/**
 * This callback is invoked by the library when a new message or request or response is received.
 */
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer)
{
  if(transfer->transfer_type == CanardTransferTypeResponse)
  {
    // Add response handlers here
  }
  else if(transfer->transfer_type == CanardTransferTypeRequest)
  {
    // Add request handlers here

    // // Handler: Get Node Info
    // if(transfer->data_type_id == GET_NODE_INFO_DATA_TYPE_ID)
    // {
    //   // TODO: node info handler
    // }
  }
  else if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    // Add braodcast handlers here
  }
}

/**
 * This callback is invoked by the library when it detects beginning of a new transfer on the bus that can be received
 * by the local node.
 * If the callback returns true, the library will receive the transfer.
 * If the callback returns false, the library will ignore the transfer.
 * All transfers that are addressed to other nodes are always ignored.
 */
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id)
{
  (void)source_node_id;

  static bool accept_transfer;

  accept_transfer = false;

  if(transfer_type == CanardTransferTypeResponse)
  {
    // Add response callbacks here
  }
  else if(transfer_type == CanardTransferTypeRequest)
  {
    // Add request callbacks here

    // // Handler: Node Info
    // if(data_type_id == NODE_INFO_DATA_TYPE_ID)
    // {
    //   *out_data_type_signature = NODE_INFO_DATA_TYPE_SIGNATURE;
    //   accept_transfer = true;
    // }
  }
  else if(transfer_type == CanardTransferTypeBroadcast)
  {
    // Add broadcast callbacks here
  }

  return accept_transfer;
}
