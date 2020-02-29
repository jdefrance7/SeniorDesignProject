#include "Wing_Segment.h"

void setup()
{
  if(!init_uavcan())
  {
    // DEBUG:
  }

  update_uptime();

  if(!init_imu())
  {
    // DEBUG:
  }

  update_uptime();

  init_serial();

  update_uptime();
  print_uptime();
}

void loop()
{
  update_uptime();
  print_uptime();

  if(!update_imu())
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  if(!queue_node_status(g_node_uptime))
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  if(!queue_camera_gimbal_status(g_node_uptime))
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  if(flush_messages() < 0)
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();

  if(!cleanup_uavcan((g_node_uptime*1000)))
  {
    // DEBUG:
  }

  update_uptime();
  print_uptime();
}
