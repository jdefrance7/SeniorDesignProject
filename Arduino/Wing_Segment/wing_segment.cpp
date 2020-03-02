#include "wing_segment.h"

//##############################################################################
// APPLICATION FUNCTIONS ///////////////////////////////////////////////////////
//##############################################################################

void update_uptime() {node.uptime = millis();}

void restart()
{
  node.local_id = 1;
  node.name = &node_name;

  hardware.major        = 1;
  hardware.minor        = 0;
  hardware.unique_id    = &node_unique_id[0];
  hardware.certificate  = &node_certificate;

  software.major        = 1;
  software.minor        = 0;
  software.field_flags  = 0;
  software.vcs_commit   = 0;
  software.image_crc    = 0;

  node.mode = NODE_MODE_INITIALIZATION;

  node.health = NODE_HEALTH_OK;

  while(!init_uavcan())
  {
    update_uptime();
    node.health = NODE_HEALTH_ERROR;
  }

  node.health = NODE_HEALTH_OK;

  while(!init_imu())
  {
    update_uptime();
    node.health = NODE_HEALTH_ERROR;
  }

  node.health = NODE_HEALTH_OK;

//  while(!init_serial())
//  {
//    update_uptime();
//    node.health = NODE_HEALTH_ERROR;
//  }
//
//  node.health = NODE_HEALTH_OK;

  // Update node's uptime
  update_uptime();

  node.mode = NODE_MODE_OPERATIONAL;
}

int16_t queue_node_status(uint64_t timestamp_msec)
{
  static uint64_t msec = timestamp_msec;
  static uint16_t re_val;

  re_val = 0;

  if((timestamp_msec - msec) > (MAX_BROADCASTING_PERIOD_MS / 2))
  {
    uint8_t buffer[NODE_STATUS_MESSAGE_SIZE];

    uint16_t offset = 0;

    static uint8_t transfer_id;

    node_status(
      buffer,
      offset,
      node.uptime,
      node.health,
      node.mode
    );

    re_val = push_message(
      &g_canard,
      NODE_STATUS_DATA_TYPE_SIGNATURE,
      NODE_STATUS_DATA_TYPE_ID,
      &transfer_id,
      CANARD_TRANSFER_PRIORITY_LOW,
      &buffer,
      NODE_STATUS_MESSAGE_SIZE
    );

    msec = timestamp_msec;
  }
  return re_val;
}

int16_t queue_camera_gimbal_status(uint64_t timestamp_msec)
{
  static uint64_t msec = timestamp_msec;
  static int16_t re_val;

  re_val = 0;

  if((timestamp_msec - msec) > (MAX_BROADCASTING_PERIOD_MS / 4))
  {
    uint8_t buffer[CAMERA_GIMBAL_STATUS_MESSAGE_SIZE];

    uint16_t offset = 0;

    static uint8_t transfer_id;

    static uint8_t gimbal_id = 8; // decide how to implement this later

    float camera_orientation_x = orientation(X_AXIS);
    float camera_orientation_y = orientation(Y_AXIS);
    float camera_orientation_z = orientation(Z_AXIS);
    float camera_orientation_w = 0;

    camera_gimbal_status(
      buffer,
      offset,
      gimbal_id,
      CAMERA_MODE_GEO_POI,
      camera_orientation_x,
      camera_orientation_y,
      camera_orientation_z,
      camera_orientation_w
    );

    re_val = push_message(
      &g_canard,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_ID,
      &transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      &buffer,
      CAMERA_GIMBAL_STATUS_MESSAGE_SIZE
    );

    msec = timestamp_msec;
  }
  return re_val;
}

//##############################################################################
// DEBUG FUNCTIONS /////////////////////////////////////////////////////////////
//##############################################################################

//#if defined(SERIAL_DEBUG)
//
//bool init_serial()
//{
//  Serial.begin(SERIAL_BAUDRATE);
//
//  long timeout = millis();
//  while((millis() - timeout) < SERIAL_TIMEOUT)
//  {
//    if(Serial){return true;}
//  }
//  return false;
//}
//
//void print_uptime()
//{
//  if(Serial)
//  {
//    Serial.print("Uptime: ");
//    Serial.print(node.uptime);
//    Serial.println(" ms");
//  }
//}
//
//#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################
