#ifndef AST_CAN_H
#define AST_CAN_H

#include <stdint.h> // primative data types

#include <canard.h> // canard objects

#include <ASTCanLib.h> // driver library

#define CANARD_MEMORY_POOL_SIZE 1024

/* NOTE: canardHandleRxFrame return values
    CANARD_OK                                  0
    CANARD_ERROR_INVALID_ARGUMENT              -2
    CANARD_ERROR_OUT_OF_MEMORY                 -3
    CANARD_ERROR_NODE_ID_NOT_SET               -4
    CANARD_ERROR_INTERNAL                      -9
    CANARD_ERROR_RX_INCOMPATIBLE_PACKET        -10
    CANARD_ERROR_RX_WRONG_ADDRESS              -11
    CANARD_ERROR_RX_NOT_WANTED                 -12
    CANARD_ERROR_RX_MISSED_START               -13
    CANARD_ERROR_RX_WRONG_TOGGLE               -14
    CANARD_ERROR_RX_UNEXPECTED_TID             -15
    CANARD_ERROR_RX_SHORT_FRAME                -16
    CANARD_ERROR_RX_BAD_CRC                    -17
*/

// Canard struct
typedef struct
{
  long bitrate;
  CanardInstance canard;
  uint8_t canard_memory_pool[CANARD_MEMORY_POOL_SIZE];
} Canard;

int init_can(Canard can, uint8_t id);

int sendCanardCANFrame(CanardInstance* canard, CanardCANFrame* txf, unsigned int timeout_ms);

int readCanardCANFrame(CanardInstance* canard, CanardCANFrame* rxf, unsigned int timeout_ms);

int transmitCanardQueue(CanardInstance* canard, int timeout_ms);

void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer);
bool shouldAcceptTransfer(const CanardInstance* ins,
                                 uint64_t* out_data_type_signature,
                                 uint16_t data_type_id,
                                 CanardTransferType transfer_type,
                                 uint8_t source_node_id);

#endif // AST_CAN_H
