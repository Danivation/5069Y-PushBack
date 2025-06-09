#include "vex.h"


// wheel diameter in mm (52.62 before) (50.375 actual)
// lower number = overshoot (600 actually reports as bigger than 600)
float TrackerWheelSize = 2 * 25.4;

// Extra functions
float DeadBand(float input, float width)
{
  if (fabs(input) < width)
  {
    return 0;
  }
  return input;
}

float ToVolt(float percent)
{
  return (percent*12.0/100.0);
}

float ToMM(float degrees)
{
  float TrackerRatio = (M_PI*TrackerWheelSize)/360;
  return (degrees / TrackerRatio);  // drivetrain gear ratio
}

float toRadians(float angle)
{
  // degrees to radians
  return angle * (M_PI / 180);
}

float toDegrees(float angle)
{
  // radians to degrees
  return angle * (180 / M_PI);
}

float reduce_0_to_360(float angle)
{
  // reduce to 0-360
  while (angle < 0) angle += 360;
  while (angle >= 360) angle -= 360;
  return angle;
}

float reduce_radians(float angle)
{
  // reduce to 0-2pi
  while (angle < 0) angle += 2 * M_PI;
  while (angle >= 2 * M_PI) angle -= 2 * M_PI;
  return angle;
}

float trueHeading()
{
  // rotation is -inf to +inf, so averaging them will not have issues of 360-0 rollover
  // reducing back to 0-360 afterwards makes sure its in a good format for everything else
  return reduce_0_to_360((Inertial1.rotation()/0.99531 + Inertial2.rotation()/0.99086) / 2);
}

float trueRotation()
{
  // returns -inf to +inf true rotation
  return ((Inertial1.rotation() + Inertial2.rotation()) / 2);
}