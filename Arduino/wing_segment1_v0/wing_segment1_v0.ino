/*  
  Implements the following tasks for the purpose of sending IMU orientation over CAN bus via UAVCAN protocol.
  
    Task 0 - Setup
      Initializes all hardware and software modules according to configuration values defined below.

    Task 1 - Update IMU
      Updates the IMU processing filters for the LSM9DS1 and NXP_FXOS_FXAS.

    Task 2 - Send Node Status
      Sends a UAVCAN formatted Node Status broadcast onto the CAN bus.

    Task 3 - Send Orientation
      Sends orientation data onto the CAN bus in one of three ways:
        1. Sends a UAVCAN formatted Log Message broadcast onto the CAN bus.
        2. Sends a UAVCAN formatted Camera Gimbal Status broadcast onto the CAN bus.
        3. Sends a series of UAVCAN formatted Key Value broadcasts onto the CAN bus.

    Task 4 - Check Memory Pool
      Checks the memory usage statistics of the Canard memory pool.
*/

// Serial Debugging
#define SERIAL_DEBUG // comment out for production, uncomment for debug information
#define SERIAL_BAUDRATE 115200

// Select Orientation Message Type
//#define SEND_ORIENTATION_AS_LOG_MESSAGE
//#define SEND_ORIENTATION_AS_CAMERA_GIMBAL_STATUS
#define SEND_ORIENTATION_AS_KEY_VALUE
//#define SEND_ORIENTATION_AS_ANGULAR_COMMAND

// Core Arduino Library
#include <Arduino.h>

// Custom LED Library
#include <led.h>

// Select IMU Sensor
#include <bno055.h>
// #include <lsm9ds1.h>
// #include <nxp_fxos_fxas.h>

// Select CAN Driver
// #include <avr_can_v0.h>
#include <ast_can_v0.h>

// UAVCAN Version
#include <uavcan_v0.h>

// LED Toggle Rate (ms)
#define LED_TOGGLE 300

// CAN Bitrate
//#define CAN_BITRATE BITRATE_125_KBPS  // AVR_CAN
#define CAN_BITRATE 1000000            // AST_CAN

// CAN Module Timeout (ms)
#define TRANSMIT_TIMEOUT 5

// CAN Inter-Frame Delay (ms)
#define TRANSMIT_DELAY 10

#define WING_SEGMENT  '1'

// Node Information
#define NODE_ID   21
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

// Task Periods (ms)
#define UPDATE_IMU_PERIOD_MS        (1000 / FILTER_UPDATE_RATE_HZ)
#define SEND_NODE_STATUS_PERIOD_MS  (MAX_BROADCASTING_PERIOD_MS / 2)
#define SEND_ORIENTATION_PERIOD_MS  (100)
#define CLEANUP_UAVCAN_PERIOD_MS    (CANARD_RECOMMENDED_STALE_TRANSFER_CLEANUP_INTERVAL_USEC / 1000)

// UAVCAN Node Instance
UavcanNode node;

// Canard Can Instance
Canard can;

// Builtin LED
LED led;

// Return value for error handling
int reVal;

void setup()
{
  // Init LED
  led = LED();

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
  init_can(&can, node.id);
  while(can.canard.node_id == 0)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Initialize IMU
  while(init_imu() != 0)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Serial debugging
  #if defined(SERIAL_DEBUG)

  // Initialize Serial
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Print Wing Segment Config
  Serial.println("\nWing Segment Config");
  printCanard(&can);
  printNode(&node);

  // Let the user read the info
  delay(1000);

  #endif // SERIAL_DEBUG
  
  // Initialization Complete!
  node.status.mode = MODE_OPERATIONAL;
  node.status.uptime_sec = millis()/1000;
}

void loop()
{
  /*
    Task: Update IMU
    Note: IMU's running AHRS filters need to be updated for calculations.
  */
  static uint64_t update_imu_time = millis();
  if((millis() - update_imu_time) >= UPDATE_IMU_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nUpdate IMU - Period: "); Serial.println((long)(millis()-update_imu_time));
    #endif

    // Update the IMU's filters
    reVal = update_imu();

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nUpdate IMU - Value: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Update Failure
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Update Successful
    else
    {
      led.off();
    }

    // Update time reference
    update_imu_time = millis();
  }

  /*
    Task: Send Node Status
    Note: This message must be sent at least once every second.
  */
  static uint64_t send_node_status_time = millis();
  if((millis() - send_node_status_time) >= SEND_NODE_STATUS_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nNode Status - Period: "); Serial.println((long)(millis()-send_node_status_time));
    #endif

    // Update uptime
    node.status.uptime_sec = millis()/1000;

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printNodeStatus(&node.status);
    #endif // SERIAL_DEBUG

    // Unique ID for node status transfers
    static uint8_t node_status_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(NODE_STATUS_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    reVal = encode_node_status(buffer, sizeof(buffer), 0, node.status);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nNode Status - Encoding: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Encoding failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Encoding successful
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nNode Status - Buffer: ");
      for(int n = 0; n < (reVal+7)/8; n++)
      {
        Serial.print(buffer[n], HEX);
        if(n == (sizeof(buffer)-1))
        {
          Serial.print("\n");
          break;
        }
        Serial.print(",");
      }
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Format and push message frame(s) onto Canard queue
    reVal = canardBroadcast(
      &can.canard,
      NODE_STATUS_DATA_TYPE_SIGNATURE,
      NODE_STATUS_DATA_TYPE_ID,
      &node_status_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      sizeof(buffer)
    );

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nNode Status - Queuing: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Queuing failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Queuing successful
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      const CanardCANFrame* txf = canardPeekTxQueue(&can.canard);
      printCanardFrame(txf);
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Transmit all frames in Canard queue
    reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nNode Status - Transmit: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Transmit failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Transmit successful
    else
    {
      led.off();
    }

    // Update time reference
    send_node_status_time = millis();
  }

  /*
    Task: Send Orientation as LogMessage
  */
  #if defined(SEND_ORIENTATION_AS_LOG_MESSAGE)
  static uint64_t send_log_message_time = millis();
  if((millis() - send_log_message_time) >= SEND_ORIENTATION_PERIOD_MS)
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

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nQuaternion - Text: "); Serial.print(text);
    #endif // SERIAL_DEBUG

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nLog Message - Period: "); Serial.println((long)(millis()-send_log_message_time));
    #endif

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
    printLogMessage(&message);
    #endif // SERIAL_DEBUG

    // Unique ID for log message transfers
    static uint8_t log_message_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[(LOG_MESSAGE_DATA_TYPE_SIZE / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    reVal = encode_log_message(buffer, sizeof(buffer), 0, message);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nLog Message - Encoding: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Encoding failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Encoding successful
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nLog Message - Buffer: ");
      for(unsigned int n = 0; n < (reVal+7)/8; n++)
      {
        Serial.print(buffer[n], HEX);
        if(n == (sizeof(buffer)-1))
        {
          Serial.print("\n");
          break;
        }
        Serial.print(",");
      }
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Payload length in bytes (rounded up)
    uint16_t payload_len = (reVal + 7) / 8;

    // Format and push message frame(s) onto Canard queue
    reVal = canardBroadcast(
      &can.canard,
      LOG_MESSAGE_DATA_TYPE_SIGNATURE,
      LOG_MESSAGE_DATA_TYPE_ID,
      &log_message_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      payload_len
    );

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nLog Message - Queuing: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Queuing failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Queuing successful
    else
    {
      led.off();
    }

    // Transmit all frames in Canard queue
    reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nLog Message - Transmit: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Transmit failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Transmit successful
    else
    {
      led.off();
    }

    // Update time reference
    send_log_message_time = millis();
  }
  #endif

  /*
    Task: Send Orientation as CameraGimbalStatus
  */
  #if defined(SEND_ORIENTATION_AS_CAMERA_GIMBAL_STATUS)
  static uint64_t send_camera_gimbal_status = millis();
  if((millis() - send_camera_gimbal_status) >= SEND_ORIENTATION_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nCamera Gimbal Status - Period: "); Serial.println((long)(millis()-send_camera_gimbal_status));
    #endif

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
    printCameraGimbalStatus(&camera);
    #endif // SERIAL_DEBUG

    // Unique ID for camera gimbal status transfers
    static uint8_t camera_gimbal_status_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[((CAMERA_GIMBAL_STATUS_DATA_TYPE_SIZE + 7) / 8)];

    // Clear data field buffer
    memset(buffer, 0, sizeof(buffer));

    // Encode data field buffer
    reVal = encode_camera_gimbal_status(buffer, sizeof(buffer), 0, camera);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nCamera Gimbal Status - Encoding: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Encoding failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Encoding successful
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nCamera Gimbal Status - Buffer: ");
      for(unsigned int n = 0; n < (reVal+7)/8; n++)
      {
        Serial.print(buffer[n], HEX);
        if(n == (sizeof(buffer)-1))
        {
          Serial.print("\n");
          break;
        }
        Serial.print(",");
      }
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Payload length in bytes (rounded up)
    uint16_t payload_len = (reVal + 7) / 8;

    // Format and push message frame(s) onto Canard queue
    reVal = canardBroadcast(
      &can.canard,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_SIGNATURE,
      CAMERA_GIMBAL_STATUS_DATA_TYPE_ID,
      &camera_gimbal_status_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      payload_len
    );

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nCamera Gimbal Status - Queuing: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Queuing failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Queuing successful
    else
    {
      led.off();
    }

    // Transmit all frames in Canard queue
    reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nCamera Gimbal Status - Transmit: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Transmit failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Transmit successful
    else
    {
      led.off();
    }

    // Update time reference
    send_camera_gimbal_status = millis();
  }
  #endif

  /*
    Task: Send Orientation as KeyValue
  */
  #if defined(SEND_ORIENTATION_AS_KEY_VALUE)
  static uint64_t send_key_value = millis();
  if((millis() - send_key_value) >= SEND_ORIENTATION_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nKey Value - Period: "); Serial.println((long)(millis()-send_key_value));
    #endif

    // Create KeyValue data structure
    KeyValue pair;

    // Payload length in bytes (set later)
    uint16_t payload_len;

    // Unique ID for camera gimbal status transfers
    static uint8_t key_value_transfer_id = 0;

    // Create data field buffer
    uint8_t buffer[((KEY_VALUE_DATA_TYPE_SIZE + 7) / 8)];

    // Iterate through quaternion values
    for(int n = 0; n < 4; n++)
    {
      // Set value
      pair.value = quaternion(n);

      // Clear key
      memset(pair.key, 0, KEY_VALUE_KEY_SIZE);

      char key[2];

      // Set key according to axis designator
      if(n == W_AXIS)
      {
        key[0] = 'W';
        key[1] = '\0';
        memcpy(pair.key, key, sizeof(key));
      }
      else if(n == X_AXIS)
      {
        key[0] = 'X';
        key[1] = '\0';
        memcpy(pair.key, key, sizeof(key));
      }
      else if(n == Y_AXIS)
      {
        key[0] = 'Y';
        key[1] = '\0';
        memcpy(pair.key, key, sizeof(key));
      }
      else if(n == Z_AXIS)
      {
        key[0] = 'Z';
        key[1] = '\0';
        memcpy(pair.key, key, sizeof(key));
      }

      // Serial debugging
      #if defined(SERIAL_DEBUG)
      printKeyValue(&pair);
      #endif

      // Clear data field buffer
      memset(buffer, 0, sizeof(buffer));

      // Encode data field buffer
      reVal = encode_key_value(buffer, sizeof(buffer), 0, pair);

      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nKey Value - Encoding: "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      // Encoding failed
      if(reVal < 0)
      {
        led.toggle(LED_TOGGLE);
      }

      // Encoding successful
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nKey Value - Buffer: ");
        for(unsigned int n = 0; n < (reVal+7)/8; n++)
        {
          Serial.print(buffer[n], HEX);
          if(n == (sizeof(buffer)-1))
          {
            Serial.print("\n");
            break;
          }
          Serial.print(",");
        }
        #endif // SERIAL_DEBUG

        led.off();
      }

      // Payload length in bytes (rounded up)
      payload_len = (reVal + 7) / 8;

      // Format and push message frame(s) onto Canard queue
      reVal = canardBroadcast(
        &can.canard,
        KEY_VALUE_DATA_TYPE_SIGNATURE,
        KEY_VALUE_DATA_TYPE_ID,
        &key_value_transfer_id,
        CANARD_TRANSFER_PRIORITY_MEDIUM,
        buffer,
        payload_len
      );

      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nKey Value - Queuing: "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      // Queuing failed
      if(reVal < 0)
      {
        led.toggle(LED_TOGGLE);
      }

      // Queuing successful
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        CanardCANFrame* txf = canardPeekTxQueue(&can.canard);
        printCanardFrame(txf);
        #endif // SERIAL_DEBUG

        led.off();
      }

      // Transmit all frames in Canard queue
      reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nKey Value - Transmit: "); Serial.println(reVal);
      #endif // SERIAL_DEBUG

      // Transmit failed
      if(reVal < 0)
      {
        led.toggle(LED_TOGGLE);
      }

      // Transmit successful
      else
      {
        led.off();
      }
    }
    send_key_value = millis();
  }
  #endif

  /*
    Task: Send Orientation as Angular Command
  */
  #if defined(SEND_ORIENTATION_AS_ANGULAR_COMMAND)
  static uint64_t send_angular_command = millis();
  if((millis() - send_angular_command) >= SEND_ORIENTATION_PERIOD_MS)
  {
    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nAngular Command - Period: "); Serial.println((long)(millis()-send_angular_command));
    #endif

    AngularCommand angles;

    angles.gimbal_id = NODE_ID; // to be set as wing segment number later

    angles.mode = 1;

    angles.quaternion_xyzw[0] = quaternion(X_AXIS);
    angles.quaternion_xyzw[1] = quaternion(Y_AXIS);
    angles.quaternion_xyzw[2] = quaternion(Z_AXIS);
    angles.quaternion_xyzw[3] = quaternion(W_AXIS);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    printAngularCommand(&angles);
    #endif // SERIAL_DEBUG

    static uint8_t angular_command_transfer_id = 0;

    uint8_t buffer[((ANGULAR_COMMAND_DATA_TYPE_SIZE + 7) / 8)];

    memset(buffer, 0, sizeof(buffer));
  
    reVal = encode_angular_command(buffer, sizeof(buffer), 0, angles);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nAngular Command - Encoding: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Encoding failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Encoding successful
    else
    {
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nAngular Command - Buffer: ");
      for(int n = 0; n < (reVal+7)/8; n++)
      {
        Serial.print(buffer[n], HEX);
        if(n == (sizeof(buffer)-1))
        {
          Serial.print("\n");
          break;
        }
        Serial.print(",");
      }
      #endif // SERIAL_DEBUG

      led.off();
    }

    // Payload length in bytes (rounded up)
    uint16_t payload_len = (reVal + 7) / 8;

    // Format and push message frame(s) onto Canard queue
    reVal = canardBroadcast(
      &can.canard,
      ANGULAR_COMMAND_DATA_TYPE_SIGNATURE,
      ANGULAR_COMMAND_DATA_TYPE_ID,
      &angular_command_transfer_id,
      CANARD_TRANSFER_PRIORITY_MEDIUM,
      buffer,
      payload_len
    );

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nAngular Command - Queuing: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Queuing failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Queuing successful
    else
    {
      led.off();
    }

    // Transmit all frames in Canard queue
    reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

    // Serial debugging
    #if defined(SERIAL_DEBUG)
    Serial.print("\nAngular Command - Transmit: "); Serial.println(reVal);
    #endif // SERIAL_DEBUG

    // Transmit failed
    if(reVal < 0)
    {
      led.toggle(LED_TOGGLE);
    }

    // Transmit successful
    else
    {
      led.off();
    }

    // Update time reference
    send_angular_command = millis();
  }
  #endif

  /*
    Task: Cleanup UAVCAN
    Note: should be done once about every second.
  */
   static uint64_t cleanup_uavcan_time = millis();
   if((millis() - cleanup_uavcan_time) > CLEANUP_UAVCAN_PERIOD_MS)
   {
     // Removes stale transfers from Canard queue based on microsecond timestamp
     canardCleanupStaleTransfers(&can.canard, 1000*millis() /* usec */);

     // Serial debugging
     #if defined(SERIAL_DEBUG)
     CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&can.canard);
     printCanardPoolAllocatorStatistics(&stats);
     #endif // SERIAL_DEBUG

     // Update time reference
     cleanup_uavcan_time = millis();
   }
}
