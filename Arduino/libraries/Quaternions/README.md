# Quaternions

Provides example functions for computing the angles between quaternions.

## Computation

The angle between two unit quaternions is defined as `\mathrm{t} = 2 * acos(Q1*inv(Q2))`

## Functions

`float angleBetweenQuaternions(x1, y1, z1, w1, x2, y2, z2, w2)` - computes angle between two quaternions passed as units

`float angleBetweenQuaternions(q1[4], q2[4])` - computes angle between two quaternions passed as arrays
