// Serial Debugging
#define SERIAL_DEBUG // uncomment for debug information
#define SERIAL_BAUDRATE 9600

// Core Arduino Library
#include <Arduino.h>

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

  // Initialization Complete!
  node.status.mode = MODE_OPERATIONAL;
  node.status.uptime_sec = millis()/1000;

  // Serial debugging
  #if defined(SERIAL_DEBUG)

  // Start Serial module
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);

  // Print initialization information
  Serial.println("\nInitialization...");
  printCanard(can);
  printNode(node);
  Serial.println("\nInitialization complete!");

  #endif // SERIAL_DEBUG
}

void loop()
{
  /*
    Task: Update IMU
    Note: IMU's running AHRS filters need to be updated for calculations.
  */
  static uint64_t update_imu_time = millis();
  if((millis() - update_imu_time) > UPDATE_IMU_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.println("Updating IMU...");
    #endif // SERIAL_DEBUG

    // Update the IMU's filters
    update_imu();

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.println("IMU updated!");
    #endif // SERIAL_DEBUG

    // Update time reference
    update_imu_time = millis();
  }

  /*
    Task: Send Node Status
    Note: This message must be sent at least once every second.
  */
  static uint64_t send_node_status_time = millis();
  if((millis() - send_node_status_time) > SEND_NODE_STATUS_PERIOD_MS)
  {
    // Update uptime
    node.status.uptime_sec = millis()/1000;

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printNodeStatus(node.status);
    #endif // SERIAL_DEBUG

    // Unique ID for node status transfers
    static uint8_t node_status_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(NODE_STATUS_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    encode_node_status(buffer, sizeof(buffer), 0, node.status);

    // Format and push message frame(s) onto Canard queue
    canardBroadcast(
      &can.canard,
      NODE_STATUS_DATA_TYPE_SIGNATURE,
      NODE_STATUS_DATA_TYPE_ID,
      &node_status_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    // Transmit all frames in Canard queue
    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.println("NodeStatus sent!");
    #endif // SERIAL_DEBUG

    // Update time reference
    send_node_status_time = millis();
  }

  /*
    Task: Send Orientation as LogMessage
  */
  static uint64_t send_log_message_time = millis();
  if((millis() - send_log_message_time) > SEND_ORIENTATION_PERIOD_MS)
  {
    // Store quaternion unit values
    float quat[4];
    quat[W_AXIS] = quaternion(W_AXIS);
    quat[X_AXIS] = quaternion(X_AXIS);
    quat[Y_AXIS] = quaternion(Y_AXIS);
    quat[Z_AXIS] = quaternion(Z_AXIS);

    // Format quaternion into comma separated list
    String text = "";
    text += String(quat[W_AXIS]); text += ",";
    text += String(quat[X_AXIS]); text += ",";
    text += String(quat[Y_AXIS]); text += ",";
    text += String(quat[Z_AXIS]); text += "\n";

    // Create LogMessage data structure
    LogMessage message;

    // Set LogMessage level
    message.level = LEVEL_INFO;

    // Set LogMessage source
    memset(message.source, 0 , LOG_MESSAGE_SOURCE_SIZE);
    memcpy(message.source, node.name, LOG_MESSAGE_TEXT_SIZE);

    // Set LogMessage text
    memset(message.text, 0, LOG_MESSAGE_TEXT_SIZE);
    memcpy(message.text, text.c_str(), text.length());

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printLogMessage(message);
    #endif // SERIAL_DEBUG

    // Unique ID for log message transfers
    static uint8_t log_message_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(LOG_MESSAGE_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    encode_log_message(buffer, sizeof(buffer), 0, message);

    // Format and push message frame(s) onto Canard queue
    canardBroadcast(
      &can.canard,
      LOG_MESSAGE_DATA_TYPE_SIGNATURE,
      LOG_MESSAGE_DATA_TYPE_ID,
      &log_message_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    // Transmit all frames in Canard queue
    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.println("LogMessage sent!");
    #endif // SERIAL_DEBUG

    // Update time reference
    send_log_message_time = millis();
  }

  /*
    Task: Send Orientation as CameraGimbalStatus
  */
  static uint64_t send_camera_gimbal_status = millis();
  if((millis() - send_camera_gimbal_status) > SEND_ORIENTATION_PERIOD_MS)
  {
    // Create CameraGimbalStatus data structure
    CameraGimbalStatus camera;

    // Set CameraGimbalStatus gimbal_id
    camera.gimbal_id = node.id;

    // Set CameraGimbalStatus mode
    camera.mode = CAMERA_GIMBAL_MODE_ORIENTATION_FIXED_FRAME;

    // Set CameraGimbalStatus orientation
    camera.camera_orientation_in_body_frame_xyzw[X_AXIS] = quaternion(X_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[Y_AXIS] = quaternion(Y_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[Z_AXIS] = quaternion(Z_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[W_AXIS] = quaternion(W_AXIS);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printCameraGimbalStatus(camera);
    ##endif // SERIAL_DEBUG

    // Unique ID for camera gimbal status transfers
    static uint8_t camera_gimbal_status_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(CAMERA_GIMBAL_STATUS_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    encode_camera_gimbal_status(buffer, sizeof(buffer), 0, camera);

    // Format and push message frame(s) onto Canard queue
    canardBroadcast(
      &can.canard,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_ID,
      &camera_gimbal_status_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    // Transmit all frames in Canard queue
    transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.println("CameraGimbmalStatus sent!");
    #endif // SERIAL_DEBUG

    // Update time reference
    send_camera_gimbal_status = millis();
  }

  /*
    Task: Cleanup UAVCAN
    Note: should be done once about every second.
  */
  static uint64_t cleanup_uavcan_time = millis();
  if((millis() - cleanup_uavcan_time) > CLEANUP_UAVCAN_PERIOD_MS)
  {
    // Removes stale transfers from Canard queue based on microsecond timestamp
    canardCleanupStaleTransfers(&can.canard, 1000*millis() /* usec */);

    // Get Canard queue stats object
    CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&can.canard);

    // Canard queue capacity in blocks
    uint16_t capacity = stats.capacity_blocks;

    // Number of blocks that are currently allocated
    uint16_t usage = stats.current_usage_blocks;

    // Maximum number of blocks used at one time since startup
    uint16_t peak = stats.peak_usage_blocks;


    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printCanardPoolAllocatorStatistics(stats);
    #endif // SERIAL_DEBUG

    // Update time reference
    cleanup_uavcan_time = millis();
  }
}
