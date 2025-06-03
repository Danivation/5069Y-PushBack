#include "vex.h"
using namespace vex;

/**
 * path jerryio outputs velocities from 0-127, default in the range from 0-100
 * format: (x, y, velocity from 0-100)
 * these are linear velocities that should be directly applied
 * make sure to have a trapezoid at the start and end
 * 
 * the actual path will be reconstructed as a new bezier from the data in the file
 * so closest t and everything will still work
 * 
 * the velocities at any given point along an imported path will be taken from the file
 * closest linear will be used from the file
 * 
 * angular velocity will be recalculated based on the imported linear
 * (this could be done in a preprocessor because its lots of math)
 * 
 * 
**/


/**
 * path.jerryio output: (x, y, velocity) (inches, inches, percent max velocity 1-100)
 * that needs to be converted to meters and m/s
**/


void ramsete::followChain(BezierChain chain, VecPose currentPose, VelocityPointList velocities)
{
  cpath autonPaths;
  for (int i = 0; i < chain.size(); i++)
  {
    CubicBezier currentBezier = chain[i];
    followChainPath(currentBezier, currentPose, velocities, 1, false);
  }
}

int minIndex = 0;
void fillDistanceTable(VecPose target, VecPose current, VelocityPointList chainVelocities)
{
  std::vector<float> distanceTable;
  distanceTable.clear();
  int minIndex_current;
  for (int i = 0; i <= chainVelocities.size()-1; i++)
  {
    // distance from calculated target to closest point in the imported list
    distanceTable.push_back(chainVelocities[i].distance(VelocityPoint(target.x, target.y, 0)));       // units
  }
  minIndex = std::distance(distanceTable.begin(), std::find(distanceTable.begin(), distanceTable.end(), *std::min_element(distanceTable.begin(), distanceTable.end())));
}

template <class T>
vex::task launch_task(T&& function) {
  //static_assert(std::is_invocable_r_v<void, T>);
  return vex::task([](void* parameters) {
    std::unique_ptr<std::function<void()>> ptr{static_cast<std::function<void()>*>(parameters)};
    (*ptr)();
    return 0;
  }, new std::function<void()>(std::forward<T>(function)));
}


VoltagePair ramsete::convertWheelSpeeds(VelocityPair input, float wheel_diameter_in, float wheelbase_width_in, float max_rpm)
{
  float linear_mps = input.linear;
  float angular_radps = input.angular;

  float wheel_radius_m = wheel_diameter_in * 0.0254f / 2.0f;
  float wheelbase_m = wheelbase_width_in * 0.0254f;
  
  float v_left_mps = linear_mps - (angular_radps * (wheelbase_m / 2.0f));
  float v_right_mps = linear_mps + (angular_radps * (wheelbase_m / 2.0f));
  
  float left_rpm = (60.0f * v_left_mps) / (2.0f * M_PI * wheel_radius_m);
  float right_rpm = (60.0f * v_right_mps) / (2.0f * M_PI * wheel_radius_m);
  
  left_rpm = vex::clamp(left_rpm, -max_rpm, max_rpm);
  right_rpm = vex::clamp(right_rpm, -max_rpm, max_rpm);

  return {left_rpm, right_rpm};
}

timer chainTimer;
const float inchesToMeters = 0.0254;
void ramsete::followChainPath(CubicBezier path, VecPose &currentPose, VelocityPointList velocities, float velocityScalar, bool reversed, float timeout, float angularScalar)
{
  float closestT, thetaHeading;
  //int minIndex;
  VecPoint closestPoint, newClosestPoint;
  VecPose target, newTarget, current;
  VelocityPair inputVel, outputVel;
  VoltagePair outputRPMs;
  
  const float maxRPM = 450.0;
  const float wheelDiameterInches = 3.25;
  const float maxSpeedMPS = (maxRPM * M_PI * wheelDiameterInches * inchesToMeters) / 60.0;

  float linearScalar = (reversed ? -1 : 1) * velocityScalar;
  float outputScalar = 1;
  float lookahead = 0;

  chainTimer.reset();
  while (chainTimer.value() <= timeout)
  {
    std::cout << "\n";
    closestT = path.findClosestT(Odometry.currentPose);
    wait(5, msec);
    std::cout << closestT << " t \n";
    closestPoint = path.getPoint(closestT);
    thetaHeading = atan2f(path.getDerivative(closestT + lookahead).x, path.getDerivative(closestT + lookahead).y);
    target = {closestPoint.x, closestPoint.y, reduce_radians((reversed ? static_cast<float>(M_PI) : 0) + thetaHeading)};        // units, specifically for heading
    current = Odometry.currentPose;
    current.theta = toRadians(trueHeading());

    //vex::task findVelocities = launch_task(std::bind(fillDistanceTable, target, current, velocities));
    fillDistanceTable(target, current, velocities);
    wait(10, msec);   // adjust to how long task takes
    //findVelocities.stop();

    float linearVel = linearScalar * (velocities[minIndex].velocity / 100.0) * maxSpeedMPS;
    float angularVel = path.getAngularVelocity(closestT, linearVel);      // CONVERT TO RADIANS/SECOND
    inputVel = {linearVel, angularVel};
    std::cout << linearVel << " linear (from path) \n" << angularVel << " angular (from path) \n";
    std::cout << current.x << ", " << current.y << ", " << current.theta << " current \n"
              << target.x << ", " << target.y << ", " << target.theta << " target \n";

    newClosestPoint = path.getPoint(closestT + lookahead);
    newTarget = {newClosestPoint.x, newClosestPoint.y, reduce_radians((reversed ? static_cast<float>(M_PI) : 0) + thetaHeading)};
    //std::cout << newTarget.x << ", " << newTarget.y << ", " << newTarget.theta << " new target \n";
    outputVel = calculate(newTarget, current, inputVel, closestT);

    outputVel.angular = vex::clamp(outputVel.angular * (reversed ? -1 : 1), -6.f, 6.f) * angularScalar;
    std::cout << outputVel.linear << " linear (from ramsete) \n" << outputVel.angular << " angular (from ramsete) \n";

    outputRPMs = convertWheelSpeeds(outputVel, wheelDiameterInches, 10.75, 450);
    L.spin(fwd, outputScalar * ToVolt((outputRPMs.left/450)*100), volt);
    R.spin(fwd, outputScalar * ToVolt((outputRPMs.right/450)*100), volt);
    if (closestT >= 1) {L.stop(hold); R.stop(hold); break;}
  }
  L.stop(hold); R.stop(hold);
  std::cout << current.x << ", " << current.y << ", " << current.theta << " final current \n"
            << target.x << ", " << target.y << ", " << target.theta << " final target \n";
}

VelocityPair ramsete::calculate(VecPose targetPoint, VecPose currentPoint, VelocityPair velocities, float t)
{
  targetPoint.x *= inchesToMeters;
  targetPoint.y *= inchesToMeters;
  currentPoint.x *= inchesToMeters;
  currentPoint.y *= inchesToMeters;
  float global_dX = targetPoint.x - currentPoint.x;       // units
  float global_dY = targetPoint.y - currentPoint.y;       // units
  float global_dTheta = targetPoint.theta - currentPoint.theta;       // units

  float eX = cos(currentPoint.theta) * global_dX + sin(currentPoint.theta) * global_dY;   // units
  float eY = -sin(currentPoint.theta) * global_dX + cos(currentPoint.theta) * global_dY;  // units
  float eTheta = global_dTheta;                                                     // units
  //std::cout << eX << " ramsete ex \n";
  //std::cout << eY << " ramsete ey \n";

  float vD = velocities.linear;   // units
  float wD = velocities.angular;  // units
  float vD2 = vD * vD;
  float wD2 = wD * wD;

  float k = 2 * this->zeta * sqrtf(wD2 + (this->b * vD2));
  //std::cout << k << " ramsete k \n";

  float vFinal = (vD * cosf(eTheta)) + (k * eX);    // units
  float wFinal = wD + (k * eTheta) + ((this->b * vD * sinf(eTheta) * eY)/eTheta);   // units

  if ((currentPoint.x == 0 && currentPoint.y == 0)) {return velocities;}
  return {vFinal, wFinal};  // units
}
