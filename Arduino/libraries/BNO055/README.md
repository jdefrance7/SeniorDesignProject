# BNO055

Provides standardized IMU functions for interfacing with the BNO055.

## Defines

`X_AXIS`, `Y_AXIS`, `Z_AXIS`, `W_AXIS` - linear axis designators

`ROLL_AXIS`, `PITCH_AXIS`, `YAW_AXIS` - rotational axis designators

`TEMPERATURE_CELSIUS`, `TEMPERATURE_KELVIN` - temperature scale designators

## Functions

`int init_imu()` - initializes the BNO055, returns 0 for success or negative error code

`int update_imu()` - updates the BNO055, always returns 0 for success

`float orientation(rotational_axis)` - returns Euler angle orientation along `rotational_axis`

`float quaternion(linear_axis)` - returns unit Quaternion value along `linear_axis`

`float angularVelocity(rotational_axis)` - returns Angular Velocity along `rotational_axis`

`float linearAcceleration(linear_axis)` - returns Linear Acceleration along `linear_axis`

`float magneticField(linear_axis)` - returns Magnetic Field Magnitude along `linear_axis`

`float temperature(temperature_scale)` - returns Temperature according to `temperature_scale`
