#include "vex.h"
using namespace vex;

const float TrackerWheelDiameter = 2; // inches

// odometry class constructor
odometry::odometry(odometry::tracker *t_horizontal, odometry::tracker *t_vertical)
{
  pt_horizontal = t_horizontal;
  pt_vertical = t_vertical;
}

// tracker class constructor
odometry::tracker::tracker(vex::rotation *sensor, float offset, float wheelSize)
{
  p_sensor = sensor;
  p_offset = offset;
  p_wheelSize = wheelSize;
}

// returns tracker position in inches (or whatever unit was used for the wheel size)
float odometry::tracker::getPosition()
{
  return (p_wheelSize * M_PI / 360) * p_sensor->position(degrees);
}

// returns offset
float odometry::tracker::getOffset()
{
  return p_offset;
}


// sets the global pose
// x and y in inches, theta in radians
void odometry::setPose(float x, float y, float theta)
{
  currentPose = {x, y, theta};
}

// returns the current pose
VecPose odometry::getPose()
{
  return currentPose;
}

// starts the tracking task
// x and y in inches, theta in absolute degrees
void odometry::startTracking(float x, float y, float theta)
{
  // set current global pose to the starting pose
  setInertial(theta, degrees);
  currentPose = {x, y, toRadians(theta)};

  // log current values as previous
  float prevVertical = pt_vertical->getPosition();
  float prevHorizontal = pt_horizontal->getPosition();
  float prevTheta = toRadians(trueHeading());

  // enable odom
  odomEnabled = true;

  while (odomEnabled)
  {
    // update frequency
    wait(10, msec);

    // deltas are changes since last update
    float deltaVertical = pt_vertical->getPosition() - prevVertical;
    float deltaHorizontal = pt_horizontal->getPosition() - prevHorizontal;
    float deltaTheta = toRadians(trueHeading()) - prevTheta;

    // find how much robot has traveled since last update
    float localX;
    float localY;

    // prevent divide by 0
    if (deltaTheta == 0)
    {
      localX = deltaHorizontal;
      localY = deltaVertical;
    }
    else
    {
      // i cant tell if the sinf is supposed to be on toRadians(trueHeading()) or on deltaTheta/2
      localX = 2 * sinf(toRadians(trueHeading())) * ((deltaHorizontal / deltaTheta) + pt_horizontal->getOffset());
      localY = 2 * sinf(toRadians(trueHeading())) * ((deltaVertical / deltaTheta) + pt_vertical->getOffset());
    }

    // magic polar coordinate conversion
    float avgTheta = prevTheta - (deltaTheta / 2);
    float polarRadius;
    float localPolarAngle;

    // prevent divide by 0
    if (localX == 0 && localY == 0)
    {
      localPolarAngle = 0;
      polarRadius = 0;
    }
    else
    {
      localPolarAngle = atan2f(localY, localX);
      polarRadius = hypotf(localY, localX);
    }
    float globalPolarAngle = localPolarAngle - avgTheta;

    // update global positions
    float deltaX = polarRadius * cosf(globalPolarAngle);
    float deltaY = polarRadius * sinf(globalPolarAngle);
    currentPose.x += deltaX;
    currentPose.y += deltaY;

    // update previous values
    prevVertical = pt_vertical->getPosition();
    prevHorizontal = pt_horizontal->getPosition();
    prevTheta = toRadians(trueHeading());
  }
}

// stops the tracking task
void odometry::stopTracking()
{
  odomEnabled = false;
}

