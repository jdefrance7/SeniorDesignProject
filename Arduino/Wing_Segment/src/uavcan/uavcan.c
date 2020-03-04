#include "uavcan.h"

/**
 * Initialization function called by application during setup
 */
bool init_uavcan()
{
  node.health   = NODE_HEALTH_OK;
  node.mode     = NODE_MODE_INITIALIZATION;
  node.uptime   = 0;

  /* Initializing the Libcanard instance (void) */
  canardInit(&g_canard,
             g_canard_memory_pool,
             sizeof(g_canard_memory_pool),
             onTransferReceived,
             shouldAcceptTransfer,
             NULL);

  /* Initializing the canard_avr backend driver (Success = 0) */
  if(canardAVRInit(CAN_BITRATE) != 0)
  {
    node.health = NODE_HEALTH_ERROR;
    return false;
  }

  /* Set unique node id (void) */
  canardSetLocalNodeID(&g_canard, node.local_id);

  /* Set node filter (Success = 1) */
  if(canardAVRConfigureAcceptanceFilters(node.local_id) != 1)
  {
    node.health = NODE_HEALTH_ERROR;
    return false;
  }

  return true;
}

/**
 * Returns g_canard memory statistics
 */
int16_t uavcan_stats()
{
  const CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&g_canard);
  const uint16_t peak_percent = (uint16_t)(100U * stats.peak_usage_blocks / stats.capacity_blocks);

  // printf("Memory pool stats: capacity %u blocks, usage %u blocks, peak usage %u blocks (%u%%)\n",
  //        stats.capacity_blocks, stats.current_usage_blocks, stats.peak_usage_blocks, peak_percent);

  /*
   * The recommended way to establish the minimal size of the memory pool is to stress-test the application and
   * record the worst case memory usage.
   */
  if (peak_percent > 70)
  {
    // DEBUG: puts("WARNING: ENLARGE MEMORY POOL");
  }
  return (int16_t)peak_percent;
}

/**
 * Clears stale transfers from tx queue
 *  Wrapper for canardCleanupStaleTransfers()
 */
int16_t cleanup_uavcan(uint64_t timestamp_usec)
{
  static uint64_t cleanup_usec = 0;

  if(cleanup_usec == 0)
  {
    cleanup_usec = timestamp_usec;
  }

  if((timestamp_usec - cleanup_usec) > CANARD_RECOMMENDED_STALE_TRANSFER_CLEANUP_INTERVAL_USEC)
  {
    canardCleanupStaleTransfers(&g_canard, timestamp_usec);
    cleanup_usec = timestamp_usec;
  }
  return 0;
}

/**
 * This callback is invoked by the library when a new message or request or response is received.
 */
void onTransferReceived(CanardInstance* ins, CanardRxTransfer* transfer)
{
  if(transfer->transfer_type == CanardTransferTypeResponse)
  {
    if(transfer->data_type_id == NODE_INFO_DATA_TYPE_ID)
    {
      uint16_t offset;

      uint8_t buffer[NODE_INFO_MESSAGE_SIZE];
      offset = 0;

      node_info(
        buffer,
        offset,
        0, /* fill in node status later */
        0, /* fill in hardware version later */
        0, /* fill in software version later */
        node.name
      );

      offset = 0;

      node_status(
        buffer,
        offset,
        node.uptime,
        node.health,
        node.mode
      );

      offset += NODE_STATUS_MESSAGE_SIZE;

      hardware_version(
        buffer,
        offset,
        hardware.major,
        hardware.minor,
        hardware.unique_id,
        hardware.certificate
      );

      offset += HARDWARE_VERSION_MESSAGE_SIZE;

      software_version(
        buffer,
        offset,
        software.major,
        software.minor,
        software.field_flags,
        software.vcs_commit,
        software.image_crc
      );

      offset += SOFTWARE_VERSION_MESSAGE_SIZE;

      const int16_t resp_res = canardRequestOrRespond(ins,
                                                     transfer->source_node_id,
                                                     NODE_INFO_DATA_TYPE_SIGNATURE,
                                                     NODE_INFO_DATA_TYPE_ID,
                                                     &transfer->transfer_id,
                                                     transfer->priority,
                                                     CanardResponse,
                                                     &buffer[0],
                                                     (uint16_t)NODE_INFO_MESSAGE_SIZE);

      if (resp_res <= 0)
      {
        // DEBUG: handle error
      }
      else
      {
        // TODO: flush_messages(&g_canard); ???
      }
    }
  }
  else if(transfer->transfer_type == CanardTransferTypeRequest)
  {
    //
  }
  else if(transfer->transfer_type == CanardTransferTypeBroadcast)
  {
    //
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
    #if defined(ACCEPTING_RESPONSES)
    {

    }
    #endif
  }
  else if(transfer_type == CanardTransferTypeRequest)
  {
    #if defined(ACCEPTING_REQUESTS)
    {
      #if defined(ACCEPT_REQUEST_NODE_INFO)
      {
        if(data_type_id == NODE_INFO_DATA_TYPE_ID)
        {
          *out_data_type_signature = NODE_INFO_DATA_TYPE_SIGNATURE;
          accept_transfer = true;
        }
      }
      #endif

      #if defined(ACCEPT_REQUEST_DATA_TYPE_INFO)
      {
        if(data_type_id == DATA_TYPE_INFO_DATA_TYPE_ID)
        {
          *out_data_type_signature = DATA_TYPE_INFO_DATA_TYPE_SIGNATURE;
          accept_transfer = true;
        }
      }
      #endif
    }
    #endif
  }
  else if(transfer_type == CanardTransferTypeBroadcast)
  {
    #if defined(ACCEPTING_BROADCASTS)
    {

    }
    #endif
  }

  return accept_transfer;
}
