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
