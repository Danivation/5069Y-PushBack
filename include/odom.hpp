#pragma once

#include "vex.h"


//extern vex::task odometry;


class odometry
{
private:
  bool odomEnabled = true;
public:
  VecPose currentPose = {0, 0, 0};
  
  odometry();
  void startTracking(float x, float y, float theta);
  void stopTracking();
  void setPose(float x, float y, float theta);
  VecPose getPose();
};
extern odometry Odometry;


class odom {
private:
  bool odomEnabled = true;

  float getVerticalChange(float& previousVerticalPosition);
  float getHorizontalChange(float& previousHorizontalPosition);
  float getChangeInRotation(float& previousRotation);
  void runOdom(float initialX, float initialY, float initialHeading);

  float ForwardTracker_center_distance = 0;
  float SidewaysTracker_center_distance = 0;
  float ForwardTracker_position = 0;
  float SideWaysTracker_position = 0;

  float prevForward = 0;
  float prevSideways = 0;
  float prevTheta = 0;



public:
  // Current pose used elsewhere
  VecPose currentOdomPose = {0, 0, 0};


  float X_position = 0;
  float Y_position = 0;
  odom();

  void startOdom(float startX, float startY, float startHeading);
  void setPose(float x, float y, float heading);
  //VecPose getPose();
  //void follow(CubicBezier curve, bool forwards, float timeout, bool splitCurve);
  void update_position(float ForwardTracker_position, float SidewaysTracker_position, float orientation_deg);

};
extern odom Odom;
