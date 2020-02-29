#ifndef DATA_TYPE_INFO_H
#define DATA_TYPE_INFO_H

// DSDL Constants
#include "dsdl.h"

// UAVCAN Formatting
#include <canard.h>

#define DATA_TYPE_INFO_REQUEST_SIZE           (671 / 8)
#define DATA_TYPE_INFO_MESSAGE_SIZE           (743 / 8)
#define DATA_TYPE_INFO_DATA_TYPE_SIGNATURE    0x1b283338a7bed2d8
#define DATA_TYPE_INFO_DATA_TYPE_ID           2

#define DATA_TYPE_INFO_NAME_SIZE              80

#define DATA_TYPE_KIND_SERVICE                0
#define DATA_TYPE_KIND_MESSAGE                1

#define DATA_TYPE_FLAG_KNOWN                  1
#define DATA_TYPE_FLAG_SUBSCRIBED             2
#define DATA_TYPE_FLAG_PUBLISHING             4
#define DATA_TYPE_FLAG_SERVING                8

int8_t data_type_info(
  uint8_t buffer[],
  uint16_t offset,
  uint64_t signature,
  uint16_t id,
  // DataTypeKind
  uint8_t flags,
  uint8_t name[]
);

/* GetDataTypeInfo DSDL

Full name: uavcan.protocol.GetDataTypeInfo
Default data type ID: 2

#
# Get the implementation details of a given data type.
#
# Request is interpreted as follows:
#  - If the field 'name' is empty, the fields 'kind' and 'id' will be used to identify the data type.
#  - If the field 'name' is non-empty, it will be used to identify the data type; the
#    fields 'kind' and 'id' will be ignored.
#

uint16 id                   # Ignored if 'name' is non-empty
DataTypeKind kind           # Ignored if 'name' is non-empty

uint8[<=80] name            # Full data type name, e.g. "uavcan.protocol.GetDataTypeInfo"

---

uint64 signature            # Data type signature; valid only if the data type is known (see FLAG_KNOWN)

uint16 id                   # Valid only if the data type is known (see FLAG_KNOWN)
DataTypeKind kind           # Ditto

uint8 FLAG_KNOWN      = 1   # This data type is defined
uint8 FLAG_SUBSCRIBED = 2   # Subscribed to messages of this type
uint8 FLAG_PUBLISHING = 4   # Publishing messages of this type
uint8 FLAG_SERVING    = 8   # Providing service of this type
uint8 flags

uint8[<=80] name            # Full data type name

*/

#endif
