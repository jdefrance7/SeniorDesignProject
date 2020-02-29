#include "Wing_Segment.h"

//##############################################################################
// APPLICATION FUNCTIONS ///////////////////////////////////////////////////////
//##############################################################################

bool queue_node_status(uint64_t timestamp_msec)
{
  static uint64_t msec = timestamp_msec;
  if((timestamp_msec - msec) > (MAX_BROADCASTING_PERIOD_MS / 2))
  {
    uint8_t buffer[NODE_STATUS_MESSAGE_SIZE];

    uint16_t offset = 0;

    static uint16_t transfer_id;

    node_status(
      buffer,
      offset,
      g_node_uptime,
      g_node_health,
      g_node_mode
    );

    push_message(
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
}

bool queue_camera_gimbal_status(uint64_t timestamp_msec)
{
  static uint64_t msec = timestamp_msec;
  if((timestamp_msec - msec) > CAMERA_GIMBAL_BROADCASTING_PERIOD_MS)
  {
    uint8_t buffer[CAMERA_GIMBAL_STATUS_MESSAGE_SIZE];

    uint16 offset = 0;

    static uint16_t transfer_id;

    static uint8_t gimbal_id = 8; // decide how to implement this later

    float16_t camera_orientation_x = orientation(X_AXIS);
    float16_t camera_orientation_y = orientation(Y_AXIS);
    float16_t camera_orientation_z = orientation(Z_AXIS);
    float16_t camera_orientation_w = 0;

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

    push_message(
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
}

//##############################################################################
// DEBUG FUNCTIONS /////////////////////////////////////////////////////////////
//##############################################################################

#if defined(SERIAL_DEBUG)

void init_serial()
{
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);
}

void print_uptime()
{
  if(Serial)
  {
    Serial.print("Uptime: ");
    Serial.print(g_node_uptime);
    Serial.println(" ms");
  }
}

#endif

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################
