// Serial Debugging
#define SERIAL_DEBUG // uncomment for debug information
#define SERIAL_BAUDRATE 9600

// Select IMU
#include <bno055.h>
// #include <lsm9ds1.h>
// #include <nxp_fxos_fxas.h>

// UAVCAN Library
#include <uavcan_v0.h>

// CAN Information
//#define CAN_BITRATE BITRATE_125_KBPS // canard_avr
#define CAN_BITRATE 500000 // ASTCanLib

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

  // Setup Node
  node.id = NODE_ID;
  memset(node.name, 0, sizeof(node.name));
  memcpy(node.name, NODE_NAME, sizeof(NODE_NAME));

  // Setup Node Status
  node.status.uptime_sec = 0;
  node.status.health = HEALTH_OK;
  node.status.mode = MODE_INITIALIZATION;
  node.status.sub_mode = 0;
  node.status.vendor_specific_status_code = 0;

  // Setup Node Hardware Version
  node.hardware.major = HARDWARE_VERSION_MAJOR;
  node.hardware.minor = HARDWARE_VERSION_MINOR;
  memset(node.hardware.unique_id, 0, sizeof(node.hardware.unique_id));
  memcpy(node.hardware.unique_id, HARDWARE_UNIQUE_ID, sizeof(HARDWARE_UNIQUE_ID));
  memset(node.hardware.certificate, 0, sizeof(node.hardware.certificate));
  memcpy(node.hardware.certificate, HARDWARE_CERTIFICATE, sizeof(HARDWARE_CERTIFICATE));

  // Setup Node Software Version
  node.software.major = SOFTWARE_VERSION_MAJOR;
  node.software.minor = SOFTWARE_VERSION_MINOR;
  node.software.optional_field_flags = SOFTWARE_OPTIONAL_FIELD_FLAGS;
  node.software.vcs_commit = SOFTWARE_VCS_COMMIT;
  node.software.image_crc = SOFTWARE_IMAGE_CRC;

  // Initialize UAVCAN
  uavcan_init(node, can);

  // Initialize IMU
  init_imu();

  // Initialization Complete!
  node.status.mode = MODE_OPERATIONAL;

  #if defined(SERIAL_DEBUG)
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial);
  Serial.println("Initialization complete!");
  #endif
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
    #if defined(SERIAL_DEBUG)
    Serial.println("Node status: ");
    Serial.print("  uptime_sec = ");
    Serial.println(node.status.uptime_sec);
    Serial.print("  health = ");
    Serial.println(node.status.health);
    Serial.print("  mode = ");
    Serial.println(node.status.mode);
    Serial.print("  sub_mode = ");
    Serial.println(node.status.sub_mode);
    Serial.print("  vendor_code = ");
    Serial.println(node.status.vendor_specific_status_code);
    #endif
    
    send_node_status(&can.canard, node.status, CANARD_TRANSFER_PRIORITY_MEDIUM);

    #if defined(SERIAL_DEBUG)
    Serial.println("NodeStatus sent!");
    #endif

    send_node_status_time = millis();
  }

  /* Task: Send Orientation

  */
  static uint64_t send_orientation_time = millis();
  if((millis() - send_orientation_time) > SEND_ORIENTATION_PERIOD_MS)
  {
    CameraGimbalStatus camera;

    camera.gimbal_id = node.id;
    camera.mode = CAMERA_GIMBAL_MODE_ORIENTATION_FIXED_FRAME;
    camera.camera_orientation_in_body_frame_xyzw[ROLL_AXIS] = orientation(ROLL_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[PITCH_AXIS] = orientation(PITCH_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[YAW_AXIS] = orientation(YAW_AXIS);
    camera.camera_orientation_in_body_frame_xyzw[3] = 0; // skip quaternion w-axis

    #if defined(SERIAL_DEBUG)
    Serial.println("CameraGimbalStatus:");
    Serial.print("  gimbal_id = ");
    Serial.println(camera.gimbal_id);
    Serial.print("  mode = ");
    Serial.println(camera.mode);
    Serial.print("  roll = ");
    Serial.println(camera.camera_orientation_in_body_frame_xyzw[ROLL_AXIS]);
    Serial.print("  pitch = ");
    Serial.println(camera.camera_orientation_in_body_frame_xyzw[PITCH_AXIS]);
    Serial.print("  yaw = ");
    Serial.println(camera.camera_orientation_in_body_frame_xyzw[YAW_AXIS]);
    #endif

    send_camera_gimbal_status(&can.canard, camera, CANARD_TRANSFER_PRIORITY_MEDIUM);

    #if defined(SERIAL_DEBUG)
    Serial.println("CameraGimbmalStatus sent!");
    #endif

    send_orientation_time = millis();
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
    Serial.println("Retreiving canard stats: ");
    Serial.print("  capacity = ");
    Serial.println(capacity);
    Serial.print("  usage = ");
    Serial.println(usage);
    Serial.print("  peak = ");
    Serial.println(peak);
    #endif

    cleanup_uavcan_time = millis();
  }
}
