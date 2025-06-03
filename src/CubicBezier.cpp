#include "vex.h"
using namespace vex;


float CubicBezier::getXPoint(float t)
{
  return this->getPoint(t).x;
}

float CubicBezier::getYPoint(float t)
{
  return this->getPoint(t).y;
}


float epsilon = 0.0000001;
float CubicBezier::estXDerivative(float t)
{
  float deltaX = this->getXPoint(t+epsilon) - this->getXPoint(t);
  //return deltaX/epsilon;
  return this->getDerivative(t).x;
}

float CubicBezier::estYDerivative(float t)
{
  float deltaY = this->getYPoint(t+epsilon) - this->getYPoint(t);
  //return deltaY/epsilon;
  return this->getDerivative(t).y;
}

float CubicBezier::estXSecondDerivative(float t)
{
  float delta2X = this->estXDerivative(t+epsilon) - this->estXDerivative(t);
  //return delta2X/epsilon;
  return this->getSecondDerivative(t).x;
}

float CubicBezier::estYSecondDerivative(float t)
{
  float delta2Y = this->estYDerivative(t+epsilon) - this->estYDerivative(t);
  //return delta2Y/epsilon;
  return this->getSecondDerivative(t).y;
}



float CubicBezier::getLinearVelocity(float t)
{
  //////////////////  insert trapezoid motion profile here  //////////////////
  float velocity = 0;
  return velocity;       // inches per second probably?
}

float CubicBezier::getAngularVelocity(float t, float linear)  // linear m/s
{
  //const float meters_per_inches = 37.3701;

  //float linear_inchesPerSecond = linear * meters_per_inches;

  // convert to meters
  float dx = 0.0254 * this->estXDerivative(t);
  float dy = 0.0254 * this->estYDerivative(t);
  float ddx = 0.0254 * this->estXSecondDerivative(t);
  float ddy = 0.0254 * this->estYSecondDerivative(t);
  
  float curvatureTop = (dx * ddy - dy * ddx);
  float curvatureBottom = powf(dx * dx + dy * dy, 1.5);
  float curvature = curvatureTop/curvatureBottom;
  float angular = linear * curvature;
  return angular;
}

/**/
float CubicBezier::findClosestT(VecPose currentPose)
{
  float t = 0;
  float densityFirst = 20;
  float densitySecond = 50;


  // iteration 1
  std::vector<VecPoint> LTable1;
  for (int i = 0; i <= densityFirst; i++)
  {
    //std::cout << "Point: " << (this->getPoint((float)i / (float)densityFirst)).x << ", " << (this->getPoint((float)i / (float)densityFirst)).y << "\n";
    LTable1.push_back(this->getPoint((float)i / (float)densityFirst));
  }
  std::vector<float> DTable1;
  for (int i = 0; i <= densityFirst; i++)
  {
    //std::cout << "Distance: " << LTable1[i].distance(VecPoint(currentPose.x, currentPose.y)) << "\n";
    DTable1.push_back(LTable1[i].distance(VecPoint(currentPose.x, currentPose.y)));
  }
  float minDist = *std::min_element(DTable1.begin(), DTable1.end());
  auto minVecPos = std::find(DTable1.begin(), DTable1.end(), minDist);
  int minIndex = densityFirst;
  if (minVecPos != DTable1.end()) {
    minIndex = std::distance(DTable1.begin(), minVecPos);
  }
  //std::cout << "first min index: " << minIndex << "\n";
  float tFirst = minIndex / (float)densityFirst;
  float tLower = tFirst - 0.5/(float)densityFirst;
  float tUpper = tFirst + 0.5/(float)densityFirst;
  //std::cout << "closest t (first): " << tFirst << "\n";
  LTable1.clear();
  DTable1.clear();

  // iteration 2
  std::vector<VecPoint> LTable2;
  for (int i = 0; i <= densitySecond; i++)       // i from .5 of a gap below the best to .5 above, spaced by d2+1
  {
    float index = tLower + (1/(float)(densityFirst*densitySecond))*i;
    //std::cout << "print float2 " << index << "\n";
    LTable2.push_back(this->getPoint(index));
  }
  std::vector<float> DTable2;
  for (int i = 0; i <= densitySecond; i++)
  {
    DTable2.push_back(LTable2[i].distance(VecPoint(currentPose.x, currentPose.y)));
  }

  //float minIndex2 = std::distance(DTable2.begin(), std::find(DTable2.begin(), DTable2.end(), *std::min_element(DTable2.begin(), DTable2.end())));

  float minDist2 = *std::min_element(DTable2.begin(), DTable2.end());
  auto minVecPos2 = std::find(DTable2.begin(), DTable2.end(), minDist2);
  float minIndex2 = densitySecond;
  if (minVecPos2 != DTable2.end()) {minIndex2 = std::distance(DTable2.begin(), minVecPos2);}
  //std::cout << "last min index: " << minIndex2 << "\n";
  t = tLower + ((float)minIndex2/(float)(densityFirst * densitySecond));
  //std::cout << "closest t (last): " << t << "\n";

  t = vex::clamp(t, 0.0f, 1.0f);
  return t;
}
/**/