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





int lbdown()
{
  wait(waitTime, sec);
  LBMotors.spin(reverse);
  return 1;
}

// Auton things
int qlbrown()
{
  LBMotorL.spin(forward, 100, rpm);
  waitUntil(LBMotorL.torque() >= 2.1);
  LBMotorL.spin(reverse);
  waitUntil(LBMotorL.torque() < 1.5);
  waitUntil(LBMotorL.torque() >= 2.1);
  LBMotorL.stop();
  return 1;
}

float MogoTime;
int mogo()
{
  wait(waitTime, sec);
  Clamp.set(true);
  std::cout << Brain.Timer.value() << " clamp \n";
  return 1;
}

//float MogoTime;
int mogo2()
{
  wait(waitTime, sec);
  Clamp.set(true);
  wait(500, msec);
  ConveyorMotor.spin(forward);
  std::cout << Brain.Timer.value() << " clamp \n";
  return 1;
}

float ConveyorTime;
int conveyor()
{
  wait(waitTime, sec);
  ConveyorMotor.spin(forward, ToVolt(100), volt);
  return 1;
}

int conveyorback()
{
  wait(waitTime, sec);
  ConveyorMotor.spin(reverse, ToVolt(100), volt);
  return 1;
}

int mogodrop()
{
  wait(waitTime, sec);
  Clamp.set(false);
  std::cout << Brain.Timer.value() << " clamp drop \n";
  return 1;
}

int lbload()
{
  wait(waitTime, sec);
  Loading = true;
  task lbload1(load);
  wait(0.7, sec);
  task::stop(lbload1);
  //LBMotorL.stop();
  Loading = false;
  return 1;
}

int lbloadconveyor()
{
  lbload();
  ConveyorMotor.spin(forward);
  return 1;
}

int mogowait()
{
  while (true)
  {
    waitUntil(RingDistance.objectDistance(mm) < 100);
    wait(500, msec);
    Clamp.set(false);
  }
}

int ringstop()
{
  while (true)
  {
    waitUntil(RingDistance.objectDistance(mm) < 45);
    ConveyorMotor.stop();
    return 1;
    break;
  }
}


int ringstop2()
{
  while (true)
  {
    waitUntil(RingColor.isNearObject());
    ConveyorMotor.stop();
    return 1;
    break;
  }
}

float locktime;
int colock()
{
  while (true)
  {
    wait(locktime, msec);
    CorrectLock = true;
  }
}

bool AJEnable = false;
int jamdetect()
{
  while (true)
  {
    if (fabs(ConveyorMotor.current()) > 0.5 && fabs(ConveyorMotor.power()) <= 0.13 && AJEnable && fabs(ConveyorMotor.velocity(rpm)) <= 50)
    {
      std::cout << "jam \n";
      ConveyorMotor.spin(reverse);
      wait(350, msec);
      ConveyorMotor.spin(forward);
      wait(150, msec);
    }
    wait(10, msec);
  }
}

int lbstop()
{
  wait(waitTime, sec);
  LBMotors.stop(hold);
  return 1;
}

int cstop()
{
  wait(waitTime, sec);
  ConveyorMotor.stop();
  return 1;
}

float waitTime = 0;
int ldoinker()
{
  wait(waitTime, sec);
  DoinkerL.set(false);
  return 1;
}

int rdoinker()
{
  wait(waitTime, sec);
  DoinkerR.set(false);
  return 1;
}

int unjam()
{
  wait(ConveyorTime, sec);
  ConveyorMotor.spin(reverse);
  wait(500, msec);
  ConveyorMotor.stop();
  wait(200, msec);
  ConveyorMotor.spin(forward);
  return 1;
}

int corner()
{
  wait(waitTime, sec);
  ConveyorMotor.spin(reverse);
  wait(300, msec);
  ConveyorMotor.stop();
  Clamp.set(false);
  return 1;
}