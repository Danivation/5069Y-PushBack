#include "vex.h"
using namespace vex;

float globalX = 0;
float globalY = 0;

const float TrackerWheelDiameter = 2; // inches

float toRadians(float angle) {
  return angle * (M_PI / 180);
}

float toDegrees(float angle) {
  return angle * (180 / M_PI);
}

float reduce_0_to_360(float angle) {
  while (angle < 0) angle += 360;
  while (angle >= 360) angle -= 360;
  return angle;
}

float reduce_radians(float angle) {
  while (angle < 0) angle += 2 * M_PI;
  while (angle >= 2 * M_PI) angle -= 2 * M_PI;
  return angle;
}

float trueHeading() {
  return reduce_0_to_360((Inertial.rotation() + Inertial2.rotation()) / 2);
}

float trueRotation() {
  return (Inertial.rotation() + Inertial2.rotation()) / 2;
}

odom::odom() {}

float odom::getVerticalChange(float &previousVerticalPosition) {
  float current = VTracker.position(degrees) * (M_PI * TrackerWheelDiameter) / 360;
  float delta = current - previousVerticalPosition;
  previousVerticalPosition = current;
  return delta;
}

float odom::getHorizontalChange(float &previousHorizontalPosition) {
  float current = HTracker.position(degrees) * (M_PI * TrackerWheelDiameter) / 360;
  float delta = current - previousHorizontalPosition;
  previousHorizontalPosition = current;
  return delta;
}

float odom::getChangeInRotation(float &previousRotation) {
  float current = toRadians(Inertial.rotation(degrees));
  float delta = current - previousRotation;
  previousRotation = current;
  return delta;
}

template <class F>
vex::task launch_task(F&& function) {
  //static_assert(std::is_invocable_r_v<void, F>);
  return vex::task([](void* parameters) {
    std::unique_ptr<std::function<void()>> ptr{static_cast<std::function<void()>*>(parameters)};
    (*ptr)();
    return 0;
  }, new std::function<void()>(std::forward<F>(function)));
}

vex::task odometry;
void odom::startOdom(float startX, float startY, float startHeading)
{
  odometry = launch_task(std::bind(&odom::runOdom, &Odom, startX, startY, startHeading));
}

void odom::setPose(float x, float y, float heading) {
  X_position = x;
  Y_position = y + 4.7;
}

void odom::runOdom(float initialX, float initialY, float initialHeading) {
  setInertial(initialHeading);
  HTracker.resetPosition();
  VTracker.resetPosition();

  ForwardTracker_center_distance = -0.28; // inches
  SidewaysTracker_center_distance = -1.71; // inches

  float ratio = M_PI * TrackerWheelDiameter / 360;

  // Initialize tracker positions and orientation
  ForwardTracker_position = VTracker.position(degrees) * ratio;
  SideWaysTracker_position = HTracker.position(degrees) * ratio;

  // Initialize odometry state
  odomEnabled = true;
  X_position = initialX;
  Y_position = initialY;
  prevForward = 0;
  prevSideways = 0;
  prevTheta = 0;


  while (odomEnabled) {
    // Update position with current tracker and heading values
    update_position(
      VTracker.position(degrees) * ratio,
      HTracker.position(degrees) * ratio,
      toRadians(trueHeading())
    );

    // Update the current odometry pose
    currentOdomPose.x = X_position;
    currentOdomPose.y = Y_position;

    // Debugging output
    Brain.Screen.printAt(0, 20, "X: %.2f Y: %.2f", X_position, Y_position);
    Brain.Screen.printAt(0, 40, "Heading: %.2f", trueHeading());

    wait(10, msec);
  }
}



void odom::update_position(float ForwardPosition_in, float SidewaysPosition_in, float heading_rad)
{
  float deltaForward = ForwardPosition_in - prevForward;
  float deltaSideways = SidewaysPosition_in - prevSideways;
  float deltaTheta = heading_rad - prevTheta;
  prevForward = ForwardPosition_in;
  prevSideways = SidewaysPosition_in;
  prevTheta = heading_rad;
  //printf("deltaF: %f, deltaS: %f, deltaT: %f\n", deltaForward, deltaSideways, deltaTheta);
  //printf("X: %f, Y: %f, theta: %f\n", X_position, Y_position, heading_rad);
  
  /**/
  if (deltaForward == ForwardPosition_in && deltaSideways == SidewaysPosition_in)
  {
    // first loop, previous values are 0
    return;
  }

  float local_deltaY, local_deltaX;
  if (deltaTheta == 0)
  {
    local_deltaY = deltaForward;
    local_deltaX = deltaSideways;
  }
  else
  {
    local_deltaY = 2*sin(deltaTheta / 2) * ((deltaForward / deltaTheta) + ForwardTracker_center_distance);  // delta (local forward)
    local_deltaX = 2*sin(deltaTheta / 2) * ((deltaSideways / deltaTheta) + SidewaysTracker_center_distance);  // delta (local sideways)
  }
  // convert local deltas to global deltas
  float avg_heading = prevTheta + (deltaTheta / 2);

  float global_deltaX = local_deltaX * cos(avg_heading) - local_deltaY * sin(avg_heading);
  float global_deltaY = local_deltaX * sin(avg_heading) + local_deltaY * cos(avg_heading);

  X_position += global_deltaX;  // update global x 
  Y_position -= global_deltaY;  // update global y
  // update pose
  currentOdomPose.x = X_position;
  currentOdomPose.y = Y_position;
  currentOdomPose.theta = heading_rad;
}




/**
void odom::update_position(float ForwardTracker_position, float SidewaysTracker_position, float orientation_deg) {
  float forward_delta = ForwardTracker_position - this->ForwardTracker_position;
  float sideways_delta = SidewaysTracker_position - this->SideWaysTracker_position;

  this->ForwardTracker_position = ForwardTracker_position;
  this->SideWaysTracker_position = SidewaysTracker_position;

  float orientation_rad = toRadians(orientation_deg);
  float prev_orientation_rad = toRadians(this->orientation_deg);
  float delta_rad = orientation_rad - prev_orientation_rad;

  this->orientation_deg = orientation_deg;

  float local_x, local_y;

  if (delta_rad == 0) {
    local_x = sideways_delta;
    local_y = forward_delta;
  } else {
    local_x = 2 * sin(delta_rad / 2) * ((sideways_delta / delta_rad) + SidewaysTracker_center_distance);
    local_y = 2 * sin(delta_rad / 2) * ((forward_delta / delta_rad) + ForwardTracker_center_distance);
  }

  float polar_angle = (local_x == 0 && local_y == 0) ? 0 : atan2(local_y, local_x);
  float polar_length = hypot(local_x, local_y);
  float global_angle = polar_angle - prev_orientation_rad - (delta_rad / 2);

  X_position += polar_length * cos(global_angle);
  Y_position += polar_length * sin(global_angle);
}
/**





VecPose odom::calculate_vertical(float offset, float current_t, float delta_vert, float delta_t)
{
  VecPose output = {0.f, 0.f, 0.f};

  // Figure out how far we've actually moved
  float local_x = delta_vert;
  float half_delta_t = 0.0;
  if (delta_t != 0) {
    half_delta_t = delta_t / 2.0;
    float i = sin(half_delta_t) * 2.0;
    local_x = (delta_vert / delta_t - offset) * i;
  }
  float alpha = current_t - half_delta_t;
  float x = cos(alpha) * local_x;
  float y = sin(alpha) * local_x;

  // xy is calculated internally using math standard but translated to what's intuitive
  // where going forward from 0 degrees increases Y
  output.x = -y;
  output.y = x;

  return output;
}



VecPose odom::calculate_horizontal(float offset, float current_t, float delta_horiz, float delta_t)
{
  VecPose output = {0.f, 0.f, 0.f};

  // Figure out how far we've actually moved
  float local_y = delta_horiz;
  float half_delta_t = 0.0;
  if (delta_t != 0) {
    half_delta_t = delta_t / 2.0;
    float i = sin(half_delta_t) * 2.0;
    local_y = (delta_horiz / delta_t + offset) * i;
  }

  float alpha = current_t - half_delta_t;
  float x = -sin(alpha) * local_y;
  float y = cos(alpha) * local_y;

  // xy is calculated internally using math standard but translated to what's intuitive
  // where going forward from 0 degrees increases Y
  output.x = -y;
  output.y = x;

  return output;
}

void odom::tracking_task()
{
  if (Inertial.isCalibrating() || Inertial2.isCalibrating() || !odomEnabled) {
    h_last = 0.0;
    t_last = 0.0;
    l_last = 0.0;
    r_last = 0.0;
    return;
  }
  float ratio = M_PI * TrackerWheelDiameter / 360;

  // Initialize tracker positions and orientation
  ForwardTracker_position = VTracker.position(degrees) * ratio;
  //SidewaysTracker_position = HTracker.position(degrees) * ratio;

  float h_current = HTracker.position(degrees) * ratio;
  float h_track_width = SidewaysTracker_center_distance;
  
  float h_ = h_current - h_last;
  h_last = h_current;
}
/**















int Odometry::trackPosition(){
  absoluteAngle = toRadians(trueHeading());
  startAngle = M_PI/2;
  //printf("posx %f",xPosGlobal);
  startPosX=(xIn*360)/(M_PI*wheelDiameter);
  //printf("startx %f\n",startPosX);
  startPosY = (yIn*360)/(M_PI*wheelDiameter);
  yPosGlobal = startPosY;
  xPosGlobal = startPosX;
  sTrackDistance = (sDistanceInput*360)/(M_PI*wheelDiameter);
  rTrackDistance = (rDistanceInput*360)/(M_PI*wheelDiameter);
  while(1){
      //change in encoder value between iterations
      deltaR = (Right.position(deg)-prevR);
      deltaS = (Back.position(deg)- prevS);
      //printf("right pos%f\n",Right.position(deg));

      //previous encoder value
      prevR = Right.position(deg);
      prevS = Back.position(deg);
      
      //total change in encoder over all time
      totalDeltaDistR += deltaR;

      //the current heading of the robot converted to radians
      absoluteAngle=ConvertToRadians(-TurnGyroSmart.rotation(deg));


      //the change in theta between iterations
      deltaTheta = absoluteAngle - prevTheta;
      //previous angle
      prevTheta = absoluteAngle;

      //if the robot's change in theta exists then we moved in 2 axis not just 1
      if ((deltaTheta !=0)) {
          halfAngle = deltaTheta/2;
          r = deltaR/deltaTheta;
          r2 = deltaS/deltaTheta;
          //The change in pose in this frame
          deltaXLocal = 2*sin(halfAngle) *(r2-sTrackDistance);
          deltaYLocal = 2*sin(halfAngle) * (r-rTrackDistance);

      //if the angle didn't change then we only moved in one direction
      } else {
          //This is not likely to happen because drift exists
          deltaXLocal = (deltaS);
          deltaYLocal = (deltaR);
          halfAngle = 0;
      }
      avgTheta=absoluteAngle-halfAngle;
      //the change in theta of the coordinates
      //Converting the local pose change to a global one
      //It's kind of hard to explain but something to do with frames and maybe rotating coordinate axis
      deltaYGlobal = (deltaXLocal) *cos(avgTheta) + (deltaYLocal) * sin(avgTheta);
      deltaXGlobal = (deltaYLocal) * cos(avgTheta) - (deltaXLocal) * sin(avgTheta);

      //adds the change to the original coordinates yielding new coordinates
      xPosGlobal += (deltaXGlobal);
      yPosGlobal += (deltaYGlobal);
      printf("Posx: %f\n", xPosGlobal);
      printf("Posy: %f\n", yPosGlobal);
      //draws coordinate grid on brain
      draw();

      task::sleep(10);
  }
  return 1;
}

double Odometry::getXPosGlobal(){
  return xPosGlobal;
}
double Odometry::getYPosGlobal(){
  return yPosGlobal;
}
double Odometry::getDeltaXG(){
  return deltaXGlobal;
}
double Odometry::getDeltaYG(){
  return deltaYGlobal;
}
double Odometry::getAbsoluteAngle(){
  return absoluteAngle;
}
double Odometry::getPrevTheta(){
  return prevTheta;
}
void Odometry::setXIn(double i){
  xIn=i;
}
void Odometry::setYIn(double i){
  yIn=i;
}
void Odometry::setAng(double a){
  startAngle=a;
}
void Odometry::setSDistInput(double i){
  sDistanceInput=i;
}
void Odometry::setRDistInput(double i){
  rDistanceInput=i;
}

//to use Odometry odom;
//task odom.trackposition;

/**/
















