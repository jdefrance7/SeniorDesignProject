#include "bno055.h"

// Sensor Instance
Adafruit_BNO055 bno = Adafruit_BNO055();

// Initialization Function
bool init_imu(void)
{
  // Init sensor (Success = true, Failure = false)
  if(!bno.begin())
  {
    return ERR_SENSOR_FAILURE;
  }

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);

  return IMU_SUCCESS;
}

// Update Function
int16_t update_imu(void)
{
  return IMU_SUCCESS;
}

// Data Aquisition
float orientation(uint8_t rotational_axis)
{
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
}

float quaternion(uint8_t rotational_axis)
{
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
}

float angularVelocity(uint8_t rotational_axis)
{
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
}
float linearAcceleration(uint8_t linear_axis)
{
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
}

float magneticField(uint8_t linear_axis)
{
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
}

float temperature(uint8_t temperature_scale)
{
  static float boardTemp = 0;

  boardTemp = (float)bno.getTemp();

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
