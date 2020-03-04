#include "imu.h"

#if defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

  #if defined(FILTER_NXP_SENSOR_FUSION)
    Adafruit_NXPSensorFusion filter;
  #elif defined(FILTER_MADGWICK)
    Adafruit_Madgwick filter;
  #elif defined(FILTER_MAHONY)
    Adafruit_Mahony filter;
  #else
    #error Filter not selected
  #endif

  #if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
    Adafruit_Sensor_Calibration_EEPROM cal;
  #else
    Adafruit_Sensor_Calibration_SDFat cal;
  #endif

#endif

bool init_imu(void)
{
  #if defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    // Retreive callibration data (Success = true, Failure = false)
    if(!cal.begin())
    {
      return false;
    }

  #endif

  // Init sensors (Success = true, Failure = false)
  if(!init_sensors())
  {
    return false;
  }

  // Setup sensors references (void)
  setup_sensors();

  // Initialize filter (void)
  #if defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    filter.begin(FILTER_UPDATE_RATE_HZ);

  #endif

  // Init I2C (void)
  Wire.setClock(400000); // 400KHz

  return true;
}

// Call at frequency FILTER_UPDATE_RATE_HZ (only for LSM9DS1 & NXP_FXOS_FXAS)
int16_t update_imu(void)
{
  #if defined(SENSOR_BNO055)

    return 0;

  #elif defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    static long msec = millis();

    float gx, gy, gz;

    // Check for update condition
    if((millis() - msec) > (1000.0 / FILTER_UPDATE_RATE_HZ))
    {
      static sensors_event_t accel, gyro, mag;

      accelerometer->getEvent(&accel);
      gyroscope->getEvent(&gyro);
      magnetometer->getEvent(&mag);

      cal.calibrate(mag);
      cal.calibrate(accel);
      cal.calibrate(gyro);

      gx = gyro.gyro.x * SENSORS_RADS_TO_DPS;
      gy = gyro.gyro.y * SENSORS_RADS_TO_DPS;
      gz = gyro.gyro.z * SENSORS_RADS_TO_DPS;

      filter.update(gx, gy, gz,
                    accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
                    mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);

      msec = millis();
    }

    return 0;

  #else

    return -1;

  #endif
}

float orientation(uint8_t rotational_axis)
{
  #if defined(SENSOR_BNO055)

    sensors_event_t event;
    bno.getEvent(&event);

    switch(rotational_axis)
    {
      case ROLL_AXIS:
        return ((float)event.orientation.x);
        break;

      case PITCH_AXIS:
        return ((float)event.orientation.y);
        break;

      case YAW_AXIS:
        return ((float)event.orientation.z);
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    switch(rotational_axis)
    {
      case ROLL_AXIS:
        return (float)filter.getRoll();
        break;

      case PITCH_AXIS:
        return (float)filter.getPitch();
        break;

      case YAW_AXIS:
        return (float)filter.getYaw();
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float quaternion(uint8_t rotational_axis)
{
  #if defined(SENSOR_BNO055)

    imu::Quaternion quat = bno.getQuat();

    switch(rotational_axis)
    {
      case X_AXIS:
        return (float)quat.x();
        break;

      case Y_AXIS:
        return (float)quat.y();
        break;

      case Z_AXIS:
        return (float)quat.z();
        break;

      case W_AXIS:
        return (float)quat.w();
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    float qw, qx, qy, qz;
    filter.getQuaternion(&qw, &qx, &qy, &qz);

    switch(rotational_axis)
    {
      case X_AXIS:
        return (float)qx;
        break;

      case Y_AXIS:
        return (float)qy;
        break;

      case Z_AXIS:
        return (float)qz;
        break;

      case W_AXIS:
        return (float)qw;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float angularVelocity(uint8_t rotational_axis)
{
  #if defined(SENSOR_BNO055)

    sensors_event_t angVelocityData;
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);

    switch(rotational_axis)
    {
      case ROLL_AXIS:
        return (float)angVelocityData.gyro.x;
        break;

      case PITCH_AXIS:
        return (float)angVelocityData.gyro.y;
        break;

      case YAW_AXIS:
        return (float)angVelocityData.gyro.z;
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    sensors_event_t angVelocityData;
    gyroscope->getEvent(&angVelocityData);
    cal.calibrate(angVelocityData);

    switch(rotational_axis)
    {
      case ROLL_AXIS:
        return (float)angVelocityData.gyro.x;
        break;

      case PITCH_AXIS:
        return (float)angVelocityData.gyro.y;
        break;

      case YAW_AXIS:
        return (float)angVelocityData.gyro.z;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float linearAcceleration(uint8_t linear_axis)
{
  #if defined(SENSOR_BNO055)

    sensors_event_t linearAccelData;
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    switch(linear_axis)
    {
      case X_AXIS:
        return (float)linearAccelData.acceleration.x;
        break;

      case Y_AXIS:
        return (float)linearAccelData.acceleration.y;
        break;

      case Z_AXIS:
        return (float)linearAccelData.acceleration.z;
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    sensors_event_t accel;
    accelerometer->getEvent(&accel);
    cal.calibrate(accel);

    switch(linear_axis)
    {
      case X_AXIS:
        return (float)accel.acceleration.x;
        break;

      case Y_AXIS:
        return (float)accel.acceleration.y;
        break;

      case Z_AXIS:
        return (float)accel.acceleration.z;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float magneticField(uint8_t linear_axis)
{
  #if defined(SENSOR_BNO055)

    imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  switch(linear_axis)
  {
    case X_AXIS:
      return (float)mag.x();
      break;

    case Y_AXIS:
      return (float)mag.y();
      break;

    case Z_AXIS:
      return (float)mag.z();
      break;

    default:
      return 0;
      break;
  }

  #elif defined(SENSOR_LSM9DS1) | defined(SENSOR_NXP_FXOS_FXAS)

    sensors_event_t mag;
    magnetometer->getEvent(&mag);
    cal.calibrate(mag);

    switch(linear_axis)
    {
      case X_AXIS:
        return (float)mag.magnetic.x;
        break;

      case Y_AXIS:
        return (float)mag.magnetic.y;
        break;

      case Z_AXIS:
        return (float)mag.magnetic.z;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float temperature(uint8_t temperature_scale)
{
  static float boardTemp = 0;

  #if defined(SENSOR_BNO055)

    boardTemp = (float)bno.getTemp();

    return 0;

  #elif defined(SENSOR_LSM9DS1)

    lsm9ds1.readTemp();

    boardTemp = (float)lsm9ds1.temperature;

  #elif defined(SENSOR_NXP_FXOS_FXAS)

    return 0;

  #endif

  if(temperature_scale == TEMPERATURE_CELSIUS)
  {
    return (float)boardTemp;
  }
  else if(temperature_scale == TEMPERATURE_KELVIN)
  {
    return (float)boardTemp + 273.5;
  }
  else
  {
    return 0;
  }
}
