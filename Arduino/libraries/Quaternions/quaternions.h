#ifndef QUATERNIONS_H
#define QUATERNIONS_H

#include <Arduino.h>

float angleBetweenQuaternions(
  float w1,
  float x1,
  float y1,
  float z1,
  float w2,
  float x2
  float y2,
  float z2
)
{
  float r = (w1*w2)+(x1*(-1*x2))+(y1*(-1*y2))+(z1*(-1*z2));
  return 2*acos(r);
}

float angleBetweenQuaternions(
  float q1[4],
  float q2[4],
)
{
  float r = (q1[0]*q2[0]) + (q1[1]*(-1*q2[1])) + (q1[2]*(-1*q2[2])) + (q1[3]*(-1*q2[3]));
  return 2*acos(r);
}

#endif // QUATERNIONS_H
