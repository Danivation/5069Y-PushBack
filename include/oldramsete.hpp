#pragma once
#include "vex.h"

//#define VoltagePair std::vector<float, float>

struct VoltagePair {
  float left;
  float right;
};

class ramsete
{
public:
  float b = 2;
  float zeta = 0.7;
  ramsete()
  {
    this->b = 2;
    this->zeta = 0.7;
  }
  ramsete(float b, float zeta)
  {
    this->b = b;
    this->zeta = zeta;
  }
  void followChain(BezierChain chain, VecPose currentPose, VelocityPointList velocities);
  void followChainPath(CubicBezier path, VecPose &currentPose, VelocityPointList velocities, float velocityScalar, bool reversed, float timeout = 3, float angularScalar = 1);
  VelocityPair calculate(VecPose targetPoint, VecPose currentPoint, VelocityPair velocities, float t);
  VoltagePair convertWheelSpeeds(VelocityPair input, float wheel_diameter_in, float wheelbase_width_in, float max_rpm);
};