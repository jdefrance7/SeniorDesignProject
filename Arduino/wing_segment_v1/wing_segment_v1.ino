// Serial Debugging
#define SERIAL_DEBUG // uncomment for debug information
#define SERIAL_BAUDRATE 9600

// Select IMU Sensor
#include <bno055.h>
// #include <lsm9ds1.h>
// #include <nxp_fxos_fxas.h>

// Select CAN Driver
#include <ast_can_v1.h>

// UAVCAN Version
#include <uavcan_v1.h>

// CAN Bitrate
#define CAN_BITRATE 500000            // AST_CAN

 // CAN Timeout
#define TRANSMIT_TIMEOUT 5

// Node Information
#define NODE_ID   22
#define NODE_NAME "#.segment.wing"

// Protocol Information
#define UAVCAN_PROTOCOL_MAJOR 1
#define UAVCAN_PROTOCOL_MINOR 0

// Hardware Information
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

#define HARDWARE_UNIQUE_ID    "123456789ABCDEF"
#define HARDWARE_CERTIFICATE  "Certificate"

// Software Information
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0

#define SOFTWARE_VCS_REVISION_ID  0
#define SOFTWARE_IMAGE_CRC        0

// Task Periods
#define UPDATE_IMU_PERIOD_MS        (1000 / FILTER_UPDATE_RATE_HZ)
#define SEND_HEARTBEAT_PERIOD_MS    (1000 * MAX_PUBLICATION_PERIOD / 2)
#define SEND_ORIENTATION_PERIOD_MS  (10)

// UAVCAN Node Instance
UavcanNode node;

// Canard Can Instance
Canard can;

void setup()
{
  // Setup CAN
  can.bitrate = CAN_BITRATE;

  // Setup UAVCAN Node
  // Node ID
  {
    node.id = NODE_ID;
  }
  // Node Status
  {
    node.status.uptime = 0;
    node.status.health = HEALTH_NOMINAL;
    node.status.mode = MODE_INITIALIZATION;
    node.status.vendor_specific_status_code = 0;
  }
  // Node Info
  {
    node.info.protocol_version.major = UAVCAN_PROTOCOL_MAJOR;
    node.info.protocol_version.minor = UAVCAN_PROTOCOL_MINOR;
    node.info.hardware_version.major = HARDWARE_VERSION_MAJOR;
    node.info.hardware_version.minor = HARDWARE_VERSION_MINOR;
    node.info.software_version.major = SOFTWARE_VERSION_MAJOR;
    node.info.software_version.minor = SOFTWARE_VERSION_MINOR;
    node.info.software_vcs_revision_id = SOFTWARE_VCS_REVISION_ID;
    memset(node.info.unique_id, 0, sizeof(node.info.unique_id));
    memcpy(node.info.unique_id, HARDWARE_UNIQUE_ID, sizeof(HARDWARE_UNIQUE_ID));
    memset(node.info.name, 0, sizeof(node.info.name));
    memcpy(node.info.name, NODE_NAME, sizeof(NODE_NAME));
    node.info.software_image_crc = SOFTWARE_IMAGE_CRC;
    memset(node.info.certificate, 0, sizeof(node.info.certificate));
    memcpy(node.info.certificate, HARDWARE_CERTIFICATE, sizeof(HARDWARE_CERTIFICATE));    
  }

  // Initialize CAN
  init_can(can, node.id);

  // Initialize IMU
  init_imu();

  // Initialize Serial (debugging)
  #if defined(SERIAL_DEBUG)
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);
  Serial.println("Initialization complete!");
  #endif

  // Initialization Complete!
  node.status.mode = MODE_OPERATIONAL;
}

void loop()
{
  /* Task: Update IMU

  */
  static uint64_t update_imu_time = millis();
  if((millis() - update_imu_time) > UPDATE_IMU_PERIOD_MS)
  {
    #if defined(SERIAL_DEBUG)
    Serial.println("Updating IMU...");
    #endif

    update_imu();

    #if defined(SERIAL_DEBUG)
    Serial.println("IMU updated!");
    #endif

    update_imu_time = millis();
  }

  /* Task: Send Heartbeat

  */
  static uint64_t heartbeat_time = millis();
  if((millis() - heartbeat_time) > SEND_HEARTBEAT_PERIOD_MS)
  {
    #if defined(SERIAL_DEBUG)
    Serial.println("NodeStatus: ");
    Serial.print("  uptime_sec = ");  Serial.println(node.status.uptime);
    Serial.print("  health = ");      Serial.println(node.status.health);
    Serial.print("  mode = ");        Serial.println(node.status.mode);
    Serial.print("  vendor_code = "); Serial.println(node.status.vendor_specific_status_code);
    #endif

    static uint8_t heartbeat_transfer_id = 0;

    uint8_t buffer[(HEARTBEAT_DATA_TYPE_SIZE / 8)];
    memset(buffer, 0, sizeof(buffer));

    encode_heartbeat(buffer, sizeof(buffer), 0, node.status);

    CanardTransfer transfer;
    transfer.timestamp_usec = 1000*millis();
    transfer.priority = CanardPriorityNominal;
    transfer.transfer_kind = CanardTransferKindMessage;
    transfer.port_id = HEARTBEAT_DATA_TYPE_ID;
    // messages ignore transfer.remote_node_id
    transfer.transfer_id = heartbeat_transfer_id;
    transfer.payload_size = sizeof(buffer);
    transfer.payload = buffer;

    canardTxPush(
      &can.canard,
      &transfer
    );

    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    #if defined(SERIAL_DEBUG)
    Serial.println("NodeStatus sent!");
    #endif

    heartbeat_time = millis();
  }

  /* Task: Send Orientation as Record

  */
  static uint64_t send_orientation_time = millis();
  if((millis() - send_orientation_time) > SEND_ORIENTATION_PERIOD_MS)
  {
    Quaternion quat;
    quat.wxyz[QW_INDEX] = quaternion(W_AXIS);
    quat.wxyz[QX_INDEX] = quaternion(X_AXIS);
    quat.wxyz[QY_INDEX] = quaternion(Y_AXIS);
    quat.wxyz[QZ_INDEX] = quaternion(Z_AXIS);

    #if defined(SERIAL_DEBUG)
    Serial.println("Quaternion:");
    Serial.print("  qW = ");    Serial.println(quat.wxyz[QW_INDEX]);
    Serial.print("  qX = ");    Serial.println(quat.wxyz[QX_INDEX]);
    Serial.print("  qY = ");    Serial.println(quat.wxyz[QY_INDEX]);
    Serial.print("  qZ = ");    Serial.println(quat.wxyz[QZ_INDEX]);
    #endif

    String text = "";
    text += String(quat.wxyz[QW_INDEX], 4); text += ",";
    text += String(quat.wxyz[QX_INDEX], 4); text += ",";
    text += String(quat.wxyz[QY_INDEX], 4); text += ",";
    text += String(quat.wxyz[QZ_INDEX], 4); text += "\n";

    Record record;
    record.timestamp = 0;
    record.severity = INFO;
    memset(record.text, 0, RECORD_TEXT_SIZE);
    memcpy(record.text, text.c_str(), text.length());

    #if defined(SERIAL_DEBUG)
    Serial.println("Record: ");
    Serial.print("  timestamp = ");   Serial.println((unsigned long)record.timestamp);
    Serial.print("  severity = ");    Serial.println(record.severity);
    Serial.print("  text = ");        Serial.println((char*)record.text);
    #endif

    static uint8_t orientation_transfer_id = 0;

    uint8_t buffer[(RECORD_DATA_TYPE_SIZE / 8)];
    memset(buffer, 0, sizeof(buffer));

    encode_record(buffer, sizeof(buffer), 0, record);

    CanardTransfer transfer;
    transfer.timestamp_usec = 1000*millis();
    transfer.priority = CanardPriorityNominal;
    transfer.transfer_kind = CanardTransferKindMessage;
    transfer.port_id = RECORD_DATA_TYPE_ID;
    // messages ignore transfer.remote_node_id
    transfer.transfer_id = orientation_transfer_id;
    transfer.payload_size = sizeof(buffer);
    transfer.payload = buffer;

    canardTxPush(
      &can.canard,
      &transfer
    );

    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    #if defined(SERIAL_DEBUG)
    Serial.println("Quaternion sent!");
    #endif

    send_orientation_time = millis();
  }
}
