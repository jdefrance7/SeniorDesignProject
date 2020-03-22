#include "uavcan_v0.h"

int16_t uavcan_init(UavcanNode node, Canard can)
{
  if(node.id == 0)
  {
    // Node is not initialized
    return -1;
  }

  canardInit(
    &can.canard,
    can.canard_memory_pool,
    sizeof(can.canard_memory_pool),
    onTransferReceived,
    shouldAcceptTransfer,
    NULL
  );

  canardSetLocalNodeID(&can.canard, node.id);

  // Using Canard_AVR
  #if defined(CANARD_AVR_H)
  if(canardAVRInit(can.bitrate) != 0)
  {
    return -1;
  }

  if(canardAVRConfigureAcceptanceFilters(node.id) != 1)
  {
    return -1;
  }
  #endif

  // Using ASTCanLib
  #if defined(_CAN_LIB_H_)
  canInit(can.bitrate);
  #endif

  return 0;
}
