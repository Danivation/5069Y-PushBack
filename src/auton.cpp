#include "vex.h"

using namespace vex;
extern odometry Odometry;

void setInertial(float angle, vex::rotationUnits units)
{
  Inertial1.setHeading(angle, units);
  Inertial2.setHeading(angle, units);
  Inertial1.setRotation(angle, units);
  Inertial2.setRotation(angle, units);
}

// Auton functions
void autonNone()
{
  /**
  ColorLock = false;
  AJEnable = false;
  SortColor = &blue;
  /**/

  
  printf("final time: %f\n", Brain.Timer.value());
}
