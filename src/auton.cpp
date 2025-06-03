#include "vex.h"

using namespace vex;
//extern odom Odom;

void setInertial(float angle)
{
  Inertial1.setHeading(angle, degrees);
  Inertial2.setHeading(angle, degrees);
  Inertial1.setRotation(angle, degrees);
  Inertial2.setRotation(angle, degrees);
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
