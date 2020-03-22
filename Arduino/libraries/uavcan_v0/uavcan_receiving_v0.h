#ifndef UAVCAN_RECEIVING_H
#define UAVCAN_RECEIVING_H

#include <stdint.h>

// Select CAN Driver (also in UAVCAN & Sending)
#include <ASTCanLib.h>
// #include <canard_avr.h>

#include <canard.h>

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: canardHandleRxFrame return values
// CANARD_OK                                  0
// CANARD_ERROR_INVALID_ARGUMENT              -2
// CANARD_ERROR_OUT_OF_MEMORY                 -3
// CANARD_ERROR_NODE_ID_NOT_SET               -4
// CANARD_ERROR_INTERNAL                      -9
// CANARD_ERROR_RX_INCOMPATIBLE_PACKET        -10
// CANARD_ERROR_RX_WRONG_ADDRESS              -11
// CANARD_ERROR_RX_NOT_WANTED                 -12
// CANARD_ERROR_RX_MISSED_START               -13
// CANARD_ERROR_RX_WRONG_TOGGLE               -14
// CANARD_ERROR_RX_UNEXPECTED_TID             -15
// CANARD_ERROR_RX_SHORT_FRAME                -16
// CANARD_ERROR_RX_BAD_CRC                    -17

int8_t read_message_frame(CanardInstance canard, CanardCANFrame* rx_frame, const uint64_t timestamp);

void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id);

#ifdef __cplusplus
}
#endif

#endif // UAVCAN_RECEIVING_H
