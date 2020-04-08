#include "lsm9ds1.h"

// Sensor Instance(s)
Adafruit_LSM9DS1 lsm9ds1 = Adafruit_LSM9DS1();

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
bool init_sensors(void) {
  if (!lsm9ds1.begin()) {
    return false;
  }
  accelerometer = &lsm9ds1.getAccel();
  gyroscope = &lsm9ds1.getGyro();
  magnetometer = &lsm9ds1.getMag();

  return true;
}

void setup_sensors(void) {
  lsm9ds1.setupAccel(lsm9ds1.LSM9DS1_ACCELRANGE_2G);
  lsm9ds1.setupMag(lsm9ds1.LSM9DS1_MAGGAIN_4GAUSS);
  lsm9ds1.setupGyro(lsm9ds1.LSM9DS1_GYROSCALE_245DPS);
}

// Initialization Function
int init_imu(void)
{
  // Retreive callibration data (Success = true, Failure = false)
  if(!cal.begin())
  {
    return ERR_CALIBRATION_BEGIN;
  }
  if (!cal.loadCalibration())
  {
    return ERR_CALIBRATION_LOAD;
  }

  // Init sensors (Success = true, Failure = false)
  if(!init_sensors())
  {
    return ERR_SENSOR_FAILURE;
  }

  // Setup sensors references (return void)
  setup_sensors();

  // Initialize filter (return void)
  filter.begin(FILTER_UPDATE_RATE_HZ);

  // Init I2C (void)
  Wire.setClock(400000); // 400KHz

  return IMU_SUCCESS;
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

  return IMU_SUCCESS;
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
  static float boardTemp = 0;

  lsm9ds1.readTemp();

  boardTemp = (float)lsm9ds1.temperature;

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
