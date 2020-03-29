// Serial Debugging
#define SERIAL_DEBUG // uncomment for debug information
#define SERIAL_BAUDRATE 9600

// Select IMU Sensor
#include <bno055.h>
// #include <lsm9ds1.h>
// #include <nxp_fxos_fxas.h>

// Select CAN Driver
// #include <avr_can_v0.h>
#include <ast_can_v0.h>

// UAVCAN Version
#include <uavcan_v0.h>

// CAN Bitrate
//#define CAN_BITRATE BITRATE_125_KBPS  // AVR_CAN
#define CAN_BITRATE 500000            // AST_CAN

// CAN Timeout
#define TRANSMIT_TIMEOUT 5

// Node Information
#define NODE_ID   22
#define NODE_NAME "#.segment.wing"

// Node Hardware Information
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0

#define HARDWARE_UNIQUE_ID    "123456789ABCDEF"
#define HARDWARE_CERTIFICATE  "Certificate"

// Node Software Information
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0

#define SOFTWARE_OPTIONAL_FIELD_FLAGS 0
#define SOFTWARE_VCS_COMMIT           0
#define SOFTWARE_IMAGE_CRC            0

// Task Periods
#define UPDATE_IMU_PERIOD_MS        (1000 / FILTER_UPDATE_RATE_HZ)
#define SEND_NODE_STATUS_PERIOD_MS  (MAX_BROADCASTING_PERIOD_MS / 2)
#define SEND_ORIENTATION_PERIOD_MS  (10)
#define CLEANUP_UAVCAN_PERIOD_MS    (CANARD_RECOMMENDED_STALE_TRANSFER_CLEANUP_INTERVAL_USEC / 1000)

// UAVCAN Node Instance
UavcanNode node;

// Canard Can Instance
Canard can;

void setup()
{
  // Setup CAN
  can.bitrate = CAN_BITRATE;

  // Setup UAVCAN Node
  // Node Basics
  {
    node.id = NODE_ID;
    memset(node.name, 0, sizeof(node.name));
    memcpy(node.name, NODE_NAME, sizeof(NODE_NAME));
  }
  // Node Status
  {
    node.status.uptime_sec = 0;
    node.status.health = HEALTH_OK;
    node.status.mode = MODE_INITIALIZATION;
    node.status.sub_mode = 0;
    node.status.vendor_specific_status_code = 0;
  }
  // Node Hardware Version
  {
    node.hardware.major = HARDWARE_VERSION_MAJOR;
    node.hardware.minor = HARDWARE_VERSION_MINOR;
    memset(node.hardware.unique_id, 0, sizeof(node.hardware.unique_id));
    memcpy(node.hardware.unique_id, HARDWARE_UNIQUE_ID, sizeof(HARDWARE_UNIQUE_ID));
    memset(node.hardware.certificate, 0, sizeof(node.hardware.certificate));
    memcpy(node.hardware.certificate, HARDWARE_CERTIFICATE, sizeof(HARDWARE_CERTIFICATE));
  }
  // Node Software Version
  {
    node.software.major = SOFTWARE_VERSION_MAJOR;
    node.software.minor = SOFTWARE_VERSION_MINOR;
    node.software.optional_field_flags = SOFTWARE_OPTIONAL_FIELD_FLAGS;
    node.software.vcs_commit = SOFTWARE_VCS_COMMIT;
    node.software.image_crc = SOFTWARE_IMAGE_CRC;    
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

  /* Task: Send Node Status

  */
  static uint64_t send_node_status_time = millis();
  if((millis() - send_node_status_time) > SEND_NODE_STATUS_PERIOD_MS)
  {
    // Update uptime
    node.status.uptime_sec = 1000*millis();
    
    #if defined(SERIAL_DEBUG)
    Serial.println("NodeStatus: ");
    Serial.print("  uptime_sec = ");  Serial.println(node.status.uptime_sec);
    Serial.print("  health = ");      Serial.println(node.status.health);
    Serial.print("  mode = ");        Serial.println(node.status.mode);
    Serial.print("  sub_mode = ");    Serial.println(node.status.sub_mode);
    Serial.print("  vendor_code = "); Serial.println(node.status.vendor_specific_status_code);
    #endif

    static uint8_t node_status_transfer_id = 0;

    uint8_t buffer[(NODE_STATUS_DATA_TYPE_SIZE / 8)];
    memset(buffer, 0, sizeof(buffer));

    encode_node_status(buffer, sizeof(buffer), 0, node.status);

    canardBroadcast(
      &can.canard,
      NODE_STATUS_DATA_TYPE_SIGNATURE,
      NODE_STATUS_DATA_TYPE_ID,
      &node_status_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    #if defined(SERIAL_DEBUG)
    Serial.println("NodeStatus sent!");
    #endif

    send_node_status_time = millis();
  }

  /* Task: Send Orientation as LogMessage

  */
  static uint64_t send_log_message_time = millis();
  if((millis() - send_log_message_time) > SEND_ORIENTATION_PERIOD_MS)
  {
    float quat[4];
    quat[W_AXIS] = quaternion(W_AXIS);
    quat[X_AXIS] = quaternion(X_AXIS);
    quat[Y_AXIS] = quaternion(Y_AXIS);
    quat[Z_AXIS] = quaternion(Z_AXIS);

    String text = "";
    text += String(quat[W_AXIS]); text += ",";
    text += String(quat[X_AXIS]); text += ",";
    text += String(quat[Y_AXIS]); text += ",";
    text += String(quat[Z_AXIS]); text += "\n";

    LogMessage message;
    message.level = LEVEL_INFO;
    memset(message.source, 0 , LOG_MESSAGE_SOURCE_SIZE);
    memcpy(message.source, node.name, LOG_MESSAGE_TEXT_SIZE);
    memset(message.text, 0, LOG_MESSAGE_TEXT_SIZE);
    memcpy(message.text, text.c_str(), text.length());

    #if defined(SERIAL_DEBUG)
    Serial.println("LogMessage: ");
    Serial.print("  level = ");   Serial.println(message.level);
    Serial.print("  source = ");  Serial.println((char*)message.source);
    Serial.print("  text = ");    Serial.println((char*)message.text);
    #endif

    static uint8_t log_message_transfer_id = 0;

    uint8_t buffer[(LOG_MESSAGE_DATA_TYPE_SIZE / 8)];
    memset(buffer, 0, sizeof(buffer));

    encode_log_message(buffer, sizeof(buffer), 0, message);

    canardBroadcast(
      &can.canard,
      LOG_MESSAGE_DATA_TYPE_SIGNATURE,
      LOG_MESSAGE_DATA_TYPE_ID,
      &log_message_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    #if defined(SERIAL_DEBUG)
    Serial.println("LogMessage sent!");
    #endif

    send_log_message_time = millis();
  }

  /* Task: Send Orientation as CameraGimbalStatus

  */
  static uint64_t send_camera_gimbal_status = millis();
  if((millis() - send_camera_gimbal_status) > SEND_ORIENTATION_PERIOD_MS)
  {
    CameraGimbalStatus camera;

    camera.gimbal_id = node.id;
    camera.mode = CAMERA_GIMBAL_MODE_ORIENTATION_FIXED_FRAME;
    camera.camera_orientation_in_body_frame_xyzw[X_AXIS] = quaternion(X_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[Y_AXIS] = quaternion(Y_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[Z_AXIS] = quaternion(Z_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[W_AXIS] = quaternion(W_AXIS);

    #if defined(SERIAL_DEBUG)
    Serial.println("CameraGimbalStatus:");
    Serial.print("  id = ");    Serial.println(camera.gimbal_id);
    Serial.print("  mode = ");  Serial.println(camera.mode);
    Serial.print("  qX = ");    Serial.println(camera.camera_orientation_in_body_frame_xyzw[X_AXIS]);
    Serial.print("  qY = ");    Serial.println(camera.camera_orientation_in_body_frame_xyzw[Y_AXIS]);
    Serial.print("  qZ = ");    Serial.println(camera.camera_orientation_in_body_frame_xyzw[Z_AXIS]);
    Serial.print("  qW = ");    Serial.println(camera.camera_orientation_in_body_frame_xyzw[W_AXIS]);
    #endif

    static uint8_t camera_gimbal_status_transfer_id = 0;

    uint8_t buffer[(CAMERA_GIMBAL_STATUS_DATA_TYPE_SIZE / 8)];
    memset(buffer, 0, sizeof(buffer));

    encode_camera_gimbal_status(buffer, sizeof(buffer), 0, camera);

    canardBroadcast(
      &can.canard,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_ID,
      &camera_gimbal_status_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    #if defined(SERIAL_DEBUG)
    Serial.println("CameraGimbmalStatus sent!");
    #endif

    send_camera_gimbal_status = millis();
  }

  /* Task: Cleanup UAVCAN

  */
  static uint64_t cleanup_uavcan_time = millis();
  if((millis() - cleanup_uavcan_time) > CLEANUP_UAVCAN_PERIOD_MS)
  {
    canardCleanupStaleTransfers(&can.canard, 1000*millis() /* usec */);

    CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&can.canard);
    uint16_t capacity = stats.capacity_blocks;   ///< Pool capacity in number of blocks
    uint16_t usage = stats.current_usage_blocks; ///< Number of blocks that are currently allocated by the library
    uint16_t peak = stats.peak_usage_blocks;     ///< Maximum number of blocks used since initialization

    #if defined(SERIAL_DEBUG)
    Serial.println("Canard memory stats: ");
    Serial.print("  capacity = ");  Serial.println(capacity);
    Serial.print("  usage = ");     Serial.println(usage);
    Serial.print("  peak = ");      Serial.println(peak);
    #endif

    cleanup_uavcan_time = millis();
  }
}
