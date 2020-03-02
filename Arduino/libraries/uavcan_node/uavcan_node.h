#ifndef UAVCAN_NODE_H
#define UAVCAN_NODE_H

typedef struct
{
  uint8_t local_id;
  uint8_t health;
  uint8_t mode;
  uint64_t uptime;
  uint8_t* name;
} uavcan_node;

typedef struct
{
  uint8_t major;
  uint8_t minor;
  const uint8_t* unique_id;
  uint8_t* certificate;
} uavcan_node_hardware_version;

typedef struct
{
  uint8_t major;
  uint8_t minor;
  uint8_t field_flags;
  uint32_t vcs_commit;
  uint64_t image_crc;
} uavcan_node_software_version;

#endif // UAVCAN_NODE_H
