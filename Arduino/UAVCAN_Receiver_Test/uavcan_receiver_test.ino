#include <uavcan.h>

#define SERIAL_BAUDRATE   9600

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

  static CanardCANFrame rx_frame;

  re_val = read_message(&rx_frame, (node.uptime*1000));
  if(re_val < 0)
  {
    // DEBUG
  }
  else
  {
    // TODO: process frame
  }

  update_uptime();

  re_val = flush_messages();
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();

  re_val = cleanup_uavcan((node.uptime*1000) /* usec */);
  if(re_val < 0)
  {
    // DEBUG:
  }

  update_uptime();
}
