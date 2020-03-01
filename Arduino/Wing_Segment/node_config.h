#ifndef NODE_CONFIG_H
#define NODE_CONFIG_H

#include "uavcan_config.h"

//##############################################################################
// SETUP UAVCAN NODE INFO //////////////////////////////////////////////////////
//##############################################################################

const uint8_t node_name = "n.segment.wing"

// Node variables
node.local_id = 1;
node.health   = UAVCAN_NODE_HEALTH_OK;
node.mode     = UAVCAN_NODE_MODE_INITIALIZATION;
node.uptime   = 0;
node.temp     = UAVCAN_NODE_TEMP_OK;
node.name     = &node_name;

//##############################################################################
// SETUP UAVCAN NODE HARDWARE INFO /////////////////////////////////////////////
//##############################################################################

const uint8_t node_unique_id[16] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

const uint8_t node_certificate = "Certificate";

// Hardware version variables
hardware.major        = 1;
hardware.minor        = 0;
hardware.unique_id    = &node_unique_id;
hardware.certificate  = &node_certificate;

//##############################################################################
// SETUP UAVCAN NODE SOFTWARE INFO /////////////////////////////////////////////
//##############################################################################

// Software version variables
software.major        = 1;
software.minor        = 0;
software.field_flags  = 1;
software.vcs_commit   = GIT_HASH;
software.image_crc    = 0;

//##############################################################################
// END OF FILE /////////////////////////////////////////////////////////////////
//##############################################################################

#endif // NODE_CONFIG_H
