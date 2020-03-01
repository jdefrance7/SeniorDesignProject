#include "Wing_Segment.h"

void setup()
{
  while(!init_uavcan())
  {
    update_uptime();
    node.health = NODE_HEALTH_ERROR;
  }

  while(!init_imu())
  {
    update_uptime();
    node.health = NODE_HEALTH_ERROR;
  }

  while(!init_serial())
  {
    update_uptime();
    node.health = NODE_HEALTH_ERROR;
  }

  update_uptime();
  print_uptime();

  node.mode = NODE_MODE_OPERATIONAL;
}

void loop()
{
  static uint16_t re_val;
  static CanardCANFrame frame;

  re_val = update_imu();
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  re_val = queue_node_status(node.uptime);
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  re_val = queue_camera_gimbal_status(node.uptime);
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  re_val = flush_messages(&g_canard, frame);
  if(re_val < 0)
  {
    // DEBUG: print frame details
  }

  update_uptime();
  print_uptime();

  cleanup_uavcan((node.uptime*1000));

  update_uptime();
  print_uptime();
}
