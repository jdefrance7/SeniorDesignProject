#include "wing_segment.h"

void setup()
{
  // Run restart sequence
  restart();
}

void loop()
{
  // Return value used to check function errors
  static int16_t re_val;

  // Update IMU - recallibrates filters
  re_val = update_imu();
  if(re_val < 0)
  {
    // DEBUG:
  }

  // Update node's uptime
  update_uptime();

  // Queue Node Status - must be broadcasted at least once per second
  re_val = queue_node_status(node.uptime);
  if(re_val < 0)
  {
    // DEBUG:
  }

  // Update node's uptime
  update_uptime();

  // Queue Camera Gimbal Status - used to send wing segment's yaw/pitch/roll
  re_val = queue_camera_gimbal_status(node.uptime);
  if(re_val < 0)
  {
    // DEBUG:
  }

  // Update node's uptime
  update_uptime();

  // Flush Messages - transmits all CAN frames from tx queue of g_canard
  re_val = flush_messages();
  if(re_val < 0)
  {
    // DEBUG: print frame details
  }

  // Update node's uptime
  update_uptime();

  // Cleanup Uavcan - purges stagnant CAN frames from tx_queue
  re_val = cleanup_uavcan((node.uptime*1000) /* usec */);
  if(re_val < 0)
  {
    // DEBUG:
  }

  // Update node's uptime
  update_uptime();
}
