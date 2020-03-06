#include <uavcan.h>

#define SERIAL_BAUDRATE   9600

int16_t queue_node_status(uint64_t timestamp_msec);

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);

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
}

void loop()
{
  static int16_t re_val;

  re_val = queue_node_status(node.uptime);
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();

  re_val = flush_messages();
  if(re_val < 0)
  {
    // DEBUG: print frame details
  }

  update_uptime();

  re_val = cleanup_uavcan((node.uptime*1000) /* usec */);
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();
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
