#include "imu.h"

bool init_imu(void)
{
  #if defined(LSM9DS1_H | NXP_FXOS_FXAS)

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
  #if defined(LSM9DS1_H | NXP_FXOS_FXAS)

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

  #elif defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

    static long msec = millis();

    if((millis() - msec) > (float(1000) / FILTER_UPDATE_RATE_HZ))
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

float16_t orientation(uint8_t rotational_axis)
{
  #if defined(SENSOR_BNO055)

    sensors_event_t event;
    bno.getEvent(&event);

    switch(axis)
    {
      case ROLL_AXIS:
        return ((float16_t)event.orientation.x);
        break;

      case PITCH_AXIS:
        return ((float16_t)event.orientation.y);
        break;

      case YAW_AXIS:
        return ((float16_t)event.orientation.z);
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

    switch(axis)
    {
      case ROLL_AXIS:
        return (float16_t)filter.getRoll();
        break;

      case PITCH_AXIS:
        return (float16_t)filter.getPitch();
        break;

      case YAW_AXIS:
        return (float16_t)filter.getYaw();
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float16_t quaternion(uint8_t rotational_axis)
{
  #if defined(SENSOR_BNO055)

    imu::Quaternion quat = bno.getQuat();

    switch(axis)
    {
      case X_AXIS:
        return (float16_t)quat.x();
        break;

      case Y_AXIS:
        return (float16_t)quat.y();
        break;

      case Z_AXIS:
        return (float16_t)quat.z();
        break;

      case W_AXIS:
        return (float16_t)quat.w();
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

    float qw, qx, qy, qz;
    filter.getQuaternion(&qw, &qx, &qy, &qz);

    switch(axis)
    {
      case X_AXIS:
        return (float16_t)qx;
        break;

      case Y_AXIS:
        return (float16_t)qy;
        break;

      case Z_AXIS:
        return (float16_t)qz;
        break;

      case W_AXIS:
        return (float16_t)qw;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float16_t angularVelocity(uint8_t rotational_axis)
{
  #if defined(SENSOR_BNO055)

    sensors_event_t angVelocityData;
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);

    switch(axis)
    {
      case ROLL_AXIS:
        return (float16_t)angVelocityData->gyro.x;
        break;

      case PITCH_AXIS:
        return (float16_t)angVelocityData->gyro.y;
        break;

      case YAW_AXIS:
        return (float16_t)angVelocityData->gyro.Z;
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

    sensors_event_t angVelocityData;
    gyroscope->getEvent(&angVelocityData);
    cal.calibrate(angVelocityData);

    switch (axis)
    {
      case ROLL_AXIS:
        return (float16_t)angVelocityData.gyro.x;
        break;

      case PITCH_AXIS:
        return (float16_t)angVelocityData.gyro.y;
        break;

      case YAW_AXIS:
        return (float16_t)angVelocityData.gyro.z;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float16_t linearAcceleration(uint8_t linear_axis)
{
  #if defined(SENSOR_BNO055)

    sensors_event_t linearAccelData;
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);

    switch(axis)
    {
      case X_AXIS:
        return (float16_t)linearAccelData->acceleration.x;
        break;

      case Y_AXIS:
        return (float16_t)linearAccelData->acceleration.y;
        break;

      case Z_AXIS:
        return (float16_t)linearAccelData->acceleration.z;
        break;

      default:
        return 0;
        break;
    }

  #elif defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

    sensors_event_t accel;
    accelerometer->getEvent(&accel);
    cal.calibrate(accel);

    switch(axis)
    {
      case X_AXIS:
        return (float16_t)accel.acceleration.x;
        break;

      case Y_AXIS:
        return (float16_t)accel.acceleration.y;
        break;

      case Z_AXIS:
        return (float16_t)accel.acceleration.z;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float16_t magneticField(uint8_t linear_axis)
{
  #if defined(SENSOR_BNO055)

    imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  switch(axis)
  {
    case X_AXIS:
      return (float16_t)mag.x();
      break;

    case Y_AXIS:
      return (float16_t)mag.y();
      break;

    case Z_AXIS:
      return (float16_t)mag.z();
      break;

    default:
      return 0;
      break;
  }

  #elif defined(SENSOR_LSM9DS1 | SENSOR_NXP_FXOS_FXAS)

    sensors_event_t mag;
    magnetometer->getEvent(&mag);
    cal.calibrate(mag);

    switch(axis)
    {
      case X_AXIS:
        return (float16_t)mag.magnetic.x;
        break;

      case Y_AXIS:
        return (float16_t)mag.magnetic.y;
        break;

      case Z_AXIS:
        return (float16_t)mag.magnetic.z;
        break;

      default:
        return 0;
        break;
    }

  #else

    return 0;

  #endif
}

float16_t temperature(uint8_t temperature_scale)
{
  static float16_t boardTemp = 0;

  #if defined(SENSOR_BNO055)

    boardTemp = (float16_t)bno.getTemp();

    return 0;

  #elif defined(SENSOR_LSM9DS1)

    lsm.readTemp();

    boardTemp = (float16_t)lsm.temperature;

  #elif defined(SENSOR_NXP_FXOS_FXAS)

    return 0;

  #endif

  if(temperature_scale == TEMPERATURE_CELSIUS)
  {
    return (float16_t)boardTemp;
  }
  else if(temperature_scale == TEMPERATURE_KELVIN)
  {
    return (float16_t)boardTemp + 273.5;
  }
  else
  {
    return 0;
  }
}
