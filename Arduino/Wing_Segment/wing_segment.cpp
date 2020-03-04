#include "wing_segment.h"

//##############################################################################
// APPLICATION FUNCTIONS ///////////////////////////////////////////////////////
//##############################################################################

void update_uptime()
{
  node.uptime = millis();
}

void restart()
{
  node.local_id = LOCAL_NODE_ID;
  memcpy(node.name, LOCAL_NODE_NAME, sizeof(LOCAL_NODE_NAME));

  hardware.major        = HARDWARE_VERSION_MAJOR;
  hardware.minor        = HARDWARE_VERSION_MINOR;
  hardware.unique_id    = NODE_UNIQUE_ID;
  memcpy(hardware.certificate, NODE_CERTIFICATE, sizeof(NODE_CERTIFICATE));

  software.major        = SOFTWARE_VERSION_MAJOR;
  software.minor        = SOFTWARE_VERSION_MINOR;
  software.field_flags  = SOFTWARE_VERSION_FIELD_FLAGS;
  software.vcs_commit   = SOFTWARE_VERSION_VCS_COMMIT;
  software.image_crc    = SOFTWARE_VERSION_IMAGE_CRC;

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
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################
