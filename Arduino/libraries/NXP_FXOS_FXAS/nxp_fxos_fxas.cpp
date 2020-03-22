#include "nxp_fxos_fxas.h"

// Sensor Instance(s)
Adafruit_FXOS8700 fxos;
Adafruit_FXAS21002C fxas;

Adafruit_Sensor *accelerometer;
Adafruit_Sensor *gyroscope;
Adafruit_Sensor *magnetometer;

// Filter Instance
#if defined(FILTER_NXP_SENSOR_FUSION)

Adafruit_NXPSensorFusion filter;

#elif defined(FILTER_MADGWICK)

Adafruit_Madgwick filter;

#elif defined(FILTER_MAHONY)

Adafruit_Mahony filter;

#endif

// Cal Instance
#if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)

Adafruit_Sensor_Calibration_EEPROM cal;

#else

Adafruit_Sensor_Calibration_SDFat cal;

#endif

// Internal Functions
bool init_sensors(void)
{
  if (!fxos.begin() || !fxas.begin()) {
    return false;
  }
  accelerometer = fxos.getAccelerometerSensor();
  gyroscope = &fxas;
  magnetometer = fxos.getMagnetometerSensor();

  return true;
}

void setup_sensors(void)
{
  return;
}

// Initialization Function
bool init_imu(void)
{
  // Retreive callibration data (Success = true, Failure = false)
  if(!cal.begin())
  {
    return false;
  }
  if (!cal.loadCalibration())
  {
    return false;
  }

  // Init sensors (Success = true, Failure = false)
  if(!init_sensors())
  {
    return false;
  }

  // Setup sensors references (return void)
  setup_sensors();

  // Initialize filter (return void)
  filter.begin(FILTER_UPDATE_RATE_HZ);

  // Init I2C (void)
  Wire.setClock(400000); // 400KHz

  return true;
}

// Update Function
int16_t update_imu(void)
{
  static long msec = millis();

  float gx, gy, gz;

  // Check for update condition
  if((millis() - msec) > (1000 / FILTER_UPDATE_RATE_HZ))
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
}

// Data Aquisition
float orientation(uint8_t rotational_axis)
{
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
}

float quaternion(uint8_t rotational_axis)
{
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
}

float angularVelocity(uint8_t rotational_axis)
{
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
}

float linearAcceleration(uint8_t linear_axis)
{
  sensors_event_t linearAccelData;
  accelerometer->getEvent(&linearAccelData);
  cal.calibrate(linearAccelData);

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
}

float temperature(uint8_t temperature_scale)
{
  return 0;
}
