// Serial Debugging Enabled
#define SERIAL_DEBUG
#define SERIAL_BAUDRATE 115200

// Select Orientation Message Type
//#define SEND_ORIENTATION_AS_LOG_MESSAGE
#define SEND_ORIENTATION_AS_CAMERA_GIMBAL_STATUS
//#define SEND_ORIENTATION_AS_KEY_VALUE

// Core Arduino Library
#include <Arduino.h>

// Custom LED Library
#include <led.h>

// IMU Sensor (constants)
#include <bno055.h>

// CAN Driver
#include <ast_can_v0.h>

// UAVCAN Version
#include <uavcan_v0.h>

// LED Toggle Rate (ms)
#define LED_TOGGLE 300

// CAN Bitrate
#define CAN_BITRATE 500000

// CAN Module Timeout (ms)
#define TRANSMIT_TIMEOUT 5

// CAN Inter-Frame Delay (ms)
#define TRANSMIT_DELAY 100

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
#define SEND_NODE_STATUS_PERIOD_MS  (MAX_BROADCASTING_PERIOD_MS / 2)
#define SEND_ORIENTATION_PERIOD_MS  (1000)
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

  // Initialize CAN & UAVCAN Node
  init_can(&can, node.id);
  while(can.canard.node_id == 0)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Initialization Complete!
  node.status.mode = MODE_OPERATIONAL;
  node.status.uptime_sec = millis()/1000;

  // Serial debugging
  #if defined(SERIAL_DEBUG)

  // Start Serial module
  Serial.begin(SERIAL_BAUDRATE);
  while(!Serial)
  {
    led.toggle(LED_TOGGLE);
  }
  led.off();

  // Print initialization information
  Serial.println("\nWing Segment Config");
  delay(250);
  printCanard(&can);
  delay(250);
  printNode(&node);
  delay(250);
  Serial.println("\nInitialization complete!");
  delay(250);

  #endif
}

void loop()
{
  while(1)
  {
    static uint64_t send_node_status_time = millis();
    if((millis() - send_node_status_time) > SEND_NODE_STATUS_PERIOD_MS)
    {
      Serial.println("\n######## NODE STATUS ########");
      
        // Update uptime
      node.status.uptime_sec = millis()/1000;
  
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      printNodeStatus(&node.status);
      #endif // SERIAL_DEBUG

      Serial.println("\n######## ENCODING NODE STATUS ########");
  
      // Unique ID for node status transfers
      static uint8_t node_status_transfer_id = 0;
  
      // Create data field buffer
      uint8_t buffer[((NODE_STATUS_DATA_TYPE_SIZE + 7) / 8)];
  
      // Clear data field buffer
      memset(buffer, 0, sizeof(buffer));
  
      // Encode data field buffer
      reVal = encode_node_status(buffer, sizeof(buffer), 0, node.status);
  
      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nERROR: Failed to encode Node Status, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
  
        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nNode Status Encoded Buffer: ");
        for(unsigned int n = 0; n < sizeof(buffer); n++)
        {
          Serial.print(buffer[n], HEX);
          if(n == (sizeof(buffer)-1))
          {
            Serial.print("\n");
            break;
          }
          Serial.print(",");
        }
        Serial.print("\nNode Status Encoded Bits: "); Serial.println(reVal);
        Serial.println("\nNode Status encoding successful!");
        #endif // SERIAL_DEBUG
  
        led.off();
      }

      Serial.println("\n######## QUEUING NODE STATUS ########");
  
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
  
      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nERROR: Failed to queue Node Status, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
  
        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        CanardCANFrame* txf = canardPeekTxQueue(&can.canard);
        printCanardFrame(txf);
        Serial.println("\nNode Status queueing successful!");
        #endif // SERIAL_DEBUG
  
        led.off();
      }

      Serial.println("\n######## TRANSMITTING NODE STATUS ########");
  
      // Transmit all frames in Canard queue
      reVal = transmitCanardQueue(&can.canard, 0, TRANSMIT_TIMEOUT);
  
      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nERROR: Failed to transmit Node Status, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
  
        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.println("\nNode Status transmission successful!");
        #endif // SERIAL_DEBUG
  
        led.off();
      }
  
      // Update time reference
      send_node_status_time = millis();
    }

    #if defined(SEND_ORIENTATION_AS_LOG_MESSAGE)
    static uint64_t send_log_message_time = millis();
    if((millis() - send_log_message_time) > SEND_ORIENTATION_PERIOD_MS)
    {
      Serial.println("\n######## QUATERNION ########");
      
      // Store quaternion unit values
      float quat[4];
      quat[W_AXIS] = 0.5;
      quat[X_AXIS] = -0.1;
      quat[Y_AXIS] = 0.76;
      quat[Z_AXIS] = -0.902;
  
      // Format quaternion into comma separated list
      String text = "";
      text += String(quat[W_AXIS]); text += ",";
      text += String(quat[X_AXIS]); text += ",";
      text += String(quat[Y_AXIS]); text += ",";
      text += String(quat[Z_AXIS]); text += "\n";
  
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      Serial.print("\nQuaternion Text: "); Serial.println(text);
      #endif // SERIAL_DEBUG

      Serial.println("\n######## LOG MESSAGE ########");
  
      // Create LogMessage data structure
      LogMessage message;
  
      // Set LogMessage level
      message.level = LEVEL_INFO;
  
      // Set LogMessage source
      memset(message.source, 0 , LOG_MESSAGE_SOURCE_SIZE);
      memcpy(message.source, node.name, LOG_MESSAGE_SOURCE_SIZE);
  
      // Set LogMessage text
      memset(message.text, 0, LOG_MESSAGE_TEXT_SIZE);
      memcpy(message.text, text.c_str(), text.length());
  
      // Serial debugging
      #if defined(SERIAL_DEBUG)
      printLogMessage(&message);
      #endif // SERIAL_DEBUG

      Serial.println("\n######## ENCODING LOG MESSAGE ########");
  
      // Unique ID for log message transfers
      static uint8_t log_message_transfer_id = 0;
  
      // Create data field buffer
      uint8_t buffer[((LOG_MESSAGE_DATA_TYPE_SIZE + 7) / 8)];
  
      // Clear data field buffer
      memset(buffer, 0, sizeof(buffer));
  
      // Encode data field buffer
      reVal = encode_log_message(buffer, sizeof(buffer), 0, message);
  
      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nERROR: Failed to encode Log Message, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
  
        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nLog Message Encoded Buffer: ");
        for(unsigned int n = 0; n < sizeof(buffer); n++)
        {
          Serial.print(buffer[n], HEX);
          if(n == (sizeof(buffer)-1))
          {
            Serial.print("\n");
            break;
          }
          Serial.print(",");
        }
        Serial.print("\nLog Message Encoded Bits: "); Serial.println(reVal);
        Serial.println("\nLog Message encoding successful!");
        #endif // SERIAL_DEBUG
      }

      Serial.println("\n######## QUEUING LOG MESSAGE ########");

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
  
      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nERROR: Failed to queue Log Message, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
  
        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.println("\nLog Message queueing successful!");
        #endif // SERIAL_DEBUG
  
        led.off();
      }

      Serial.println("\n######## TRANSMITTING LOG MESSAGE ########");
  
      // Transmit all frames in Canard queue
      // reVal = transmitCanardQueue(&can.canard, TRANSMIT_TIMEOUT); // too fast for AST-CAN485 receiver

      // Hard code transfer delay
      for(CanardCANFrame* txf = NULL; (txf = canardPeekTxQueue(&(can.canard))) != NULL;)
      {
        // Print CAN frame
        printCanardFrame(txf);
        
        // Send CAN frame
        reVal = sendCanardCANFrame(&(can.canard), txf, TRANSMIT_TIMEOUT);

        // Print transmit status
        Serial.print("Transmit: "); 
        if(reVal == 0)
        {
          Serial.println("Success");          
        }
        else
        {
          Serial.println("Failure");
        }

        // Pop CAN frame from queue
        canardPopTxQueue(&(can.canard));

        // Hard code delay
        delay(TRANSMIT_DELAY);
      }
  
      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("\nERROR: Failed to transmit Log Message, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
  
        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.println("\nLogMessage transmission successful!");
        #endif // SERIAL_DEBUG
  
        led.off();
      }
  
      // Update time reference
      send_log_message_time = millis();
    }
    #endif

    #if defined(SEND_ORIENTATION_AS_CAMERA_GIMBAL_STATUS)
    static uint64_t send_camera_gimbal_status = millis();
    if((millis() - send_camera_gimbal_status) >= SEND_ORIENTATION_PERIOD_MS)
    {
      Serial.println("\n######## CAMERA GIMBAL STATUS ########");

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

      Serial.println("\n######## ENCODING CAMERA GIMBAL STATUS ########");  
      
      // Unique ID for camera gimbal status transfers
      static uint8_t camera_gimbal_status_transfer_id = 0;

      // Create data field buffer
      uint8_t buffer[((CAMERA_GIMBAL_STATUS_DATA_TYPE_SIZE + 7) / 8)];

      // Clear data field buffer
      memset(buffer, 0, sizeof(buffer));

      // Encode data field buffer
      reVal = encode_camera_gimbal_status(buffer, sizeof(buffer), 0, camera);

      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("ERROR: Failed to encode Camera Gimbal Status, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG

        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("Camera Gimbal Status Encoded Buffer: ");
        for(unsigned int n = 0; n < sizeof(buffer); n++)
        {
          Serial.print(buffer[n], HEX);
          if(n == (sizeof(buffer)-1))
          {
            Serial.print("\n");
            break;
          }
          Serial.print(",");
        }
        Serial.print("Camera Gimbal Status Encoded Bits: "); Serial.println(reVal);
        #endif // SERIAL_DEBUG
      }

      Serial.println("\n######## QUEUING CAMERA GIMBAL STATUS ########");  
      
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

      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("ERROR: Failed to queue Camera Gimbal Status, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG

        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.println("Camera Gimbal Status queueing successful.");
        #endif // SERIAL_DEBUG

        led.off();
      }
      
      Serial.println("\n######## TRANSMITTING CAMERA GIMBAL STATUS ########");  

      // Transmit all frames in Canard queue
      reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

      if(reVal < 0)
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.print("ERROR: Failed to transmit Camera Gimbal Status, reVal = "); Serial.println(reVal);
        #endif // SERIAL_DEBUG

        led.toggle(LED_TOGGLE);
      }
      else
      {
        // Serial debugging
        #if defined(SERIAL_DEBUG)
        Serial.println("Camera Gimbmal Status transmission successful!");
        #endif // SERIAL_DEBUG

        led.off();
      }

      // Update time reference
      send_camera_gimbal_status = millis();
    }
    #endif

    #if defined(SEND_ORIENTATION_AS_KEY_VALUE)
    static uint64_t send_key_value = millis();
    if((millis() - send_key_value) >= SEND_ORIENTATION_PERIOD_MS)
    {
      uint16_t payload_len;
      KeyValue pair;

      // Unique ID for camera gimbal status transfers
      static uint8_t key_value_transfer_id = 0;

      // Create data field buffer
      uint8_t buffer[((KEY_VALUE_DATA_TYPE_SIZE + 7) / 8)];

      for(int n = 0; n < 4; n++)
      {
        pair.value = quaternion(n);
        memset(pair.key, 0, KEY_VALUE_KEY_SIZE);
        if(n == W_AXIS)
        {
          memcpy(pair.key, "W", 1);
        }
        else if(n == X_AXIS)
        {
          memcpy(pair.key, "X", 1);
        }
        else if(n == Y_AXIS)
        {
          memcpy(pair.key, "Y", 1);
        }
        else if(n == Z_AXIS)
        {
          memcpy(pair.key, "Z", 1);
        }
        
        Serial.println("\n######## KEY VALUE ########");

        printKeyValue(&pair);
        
        Serial.println("\n######## ENCODING KEY VALUE ########");

        // Clear data field buffer
        memset(buffer, 0, sizeof(buffer));

        // Encode data field buffer
        reVal = encode_key_value(buffer, sizeof(buffer), 0, pair);

        if(reVal < 0)
        {
          // Serial debugging
          #if defined(SERIAL_DEBUG)
          Serial.print("ERROR: Failed to encode Camera Gimbal Status, reVal = "); Serial.println(reVal);
          #endif // SERIAL_DEBUG

          led.toggle(LED_TOGGLE);
        }
        else
        {
          // Serial debugging
          #if defined(SERIAL_DEBUG)
          Serial.print("Key Value Encoded Buffer: ");
          for(unsigned int n = 0; n < sizeof(buffer); n++)
          {
            Serial.print(buffer[n], HEX);
            if(n == (sizeof(buffer)-1))
            {
              Serial.print("\n");
              break;
            }
            Serial.print(",");
          }
          Serial.print("Key Value Encoded Bits: "); Serial.println(reVal);
          #endif // SERIAL_DEBUG

          led.off();
        }
        
        Serial.println("\n######## QUEUING KEY VALUE ########");

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

        if(reVal < 0)
        {
          // Serial debugging
          #if defined(SERIAL_DEBUG)
          Serial.print("ERROR: Failed to queue Key Value, reVal = "); Serial.println(reVal);
          #endif // SERIAL_DEBUG

          led.toggle(LED_TOGGLE);
        }
        else
        {
          // Serial debugging
          #if defined(SERIAL_DEBUG)
          CanardCANFrame* txf = canardPeekTxQueue(&can.canard);
          printCanardFrame(txf);
          Serial.println("\nKey Value queueing successful!");
          #endif // SERIAL_DEBUG

          led.off();
        }
        
        Serial.println("\n######## TRANSMITTING KEY VALUE ########");

        // Transmit all frames in Canard queue
        reVal = transmitCanardQueue(&can.canard, TRANSMIT_DELAY, TRANSMIT_TIMEOUT);

        if(reVal < 0)
        {
          // Serial debugging
          #if defined(SERIAL_DEBUG)
          Serial.print("ERROR: Failed to transmit Key Value, reVal = "); Serial.println(reVal);
          #endif // SERIAL_DEBUG

          led.toggle(LED_TOGGLE);
        }
        else
        {
          // Serial debugging
          #if defined(SERIAL_DEBUG)
          Serial.println("Key Value transmission successful!");
          #endif // SERIAL_DEBUG

          led.off();
        }
      }
      send_key_value = millis();
    }
    #endif

    static uint64_t cleanup_uavcan_time = millis();
    if((millis() - cleanup_uavcan_time) > 1000)
    {

      Serial.println("\n######## UAVCAN CLEANUP ########");
      
      // Removes stale transfers from Canard queue based on microsecond timestamp
      canardCleanupStaleTransfers(&can.canard, 1000*millis() /* usec */);
  
      // Get Canard queue stats object
      CanardPoolAllocatorStatistics stats = canardGetPoolAllocatorStatistics(&can.canard);
  
//      // Serial debugging
//      #if defined(SERIAL_DEBUG)
//      printCanardPoolAllocatorStatistics(&stats);
//      #endif // SERIAL_DEBUG
  
      // Update time reference
      cleanup_uavcan_time = millis();
    } 
  }
}
