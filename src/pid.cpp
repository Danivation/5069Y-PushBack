#include "vex.h"

using namespace vex;

// Drive control functions

bool PidOn = true;     // enables pid globally
bool PidLock = false;  // locks pid, when finished
float Dist;            // error from setpoint
float Goal;            // setpoint
float DistInt;         // integral
float PDist;           // previous error
float Power;           // final speed output
float AutoAngle;
float CorrectionError;
float Magnitude;
float LeftOffset;      // left motor scaling
float RightOffset;     // right motor scaling
float LeftCorrection;
float RightCorrection;
bool CorrectLock;
bool Linear;           // whether driving straight or turning
float StartInt = 30;   // integral start (integral limit)

float kp;
float ki;
float kd;

bool AlignLock = false;
int AColor;
float CenterDist = 0;
float akp;
float SizeRatio;
float MinWidth;
float APower = 0;

timer TimerDrive;
timer TimerTurn;
timer DriveTime;
timer TurnTime;

float DSLimit = 10;
float DSThreshold = 80;
float DNoSettleBefore = 0;

float TSLimit = 2;
float TSThreshold = 150;

bool Settled = false;

int TurnSettle()
{
  int timeSpentSettled = 0;
  Settled = false;
  while (true)
  {
    //if(fabs(Dist) <= TSLimit) // 2
    if((L.power() <= 0.4) && (R.power() <= 0.4) && (fabs(Dist) <= TSLimit))
    {
      timeSpentSettled += 5;
      wait(5, msec);
    }
    else
    {
      timeSpentSettled = 0;
      wait(5, msec);
    }
    if (timeSpentSettled >= TSThreshold) // 150
    {
      //std::cout << TimerTurn.value() << " seconds to settle (turn) \n";
      Settled = true;
      break;
    }
  }
  TimerTurn.reset();
  return 1;
}

int DriveSettle()
{
  int timeSpentSettled = 0;
  Settled = false;
  wait(DNoSettleBefore, sec);
  while (true)
  {
    if(fabs((Goal * ((M_PI*TrackerWheelSize)/360)) - (VTracker.position(turns) * M_PI * TrackerWheelSize)) <= DSLimit) // 10
    {
      timeSpentSettled += 5;
      wait(5, msec);
    }
    else
    {
      timeSpentSettled = 0;
      wait(2, msec);
    }
    if (timeSpentSettled >= DSThreshold) // 80
    {
      //std::cout << TimerDrive.value() << " seconds to settle (drive) \n";
      Settled = true;
      break;
    }
  }
  TimerDrive.reset();
  return 1;
}

int PID()
{
  PidOn = true;
  while (PidOn)
  {
    if (!PidLock)
    {
      if (Linear)
      {
        StartInt = 30;
        Dist = Goal +VTracker.position(degrees);
        CorrectionError = AutoAngle - trueHeading();

        if (trueHeading() >= (CorrectionError + 180))
        {
          CorrectionError = (AutoAngle - trueHeading())+360;
        }
        if (CorrectionError > 180)
        {
          CorrectionError = CorrectionError - 360;
        }

        if (fabs(Dist) < StartInt)
        {
          DistInt+=Dist;
        }
        if ((Dist>0 && PDist<0)||(Dist<0 && PDist>0))
        {
          DistInt = 0;
        }

        Power = kp*Dist + ki*DistInt + kd*(Dist-PDist);

        if (!CorrectLock)
        {
          LeftCorrection = LeftOffset + (CorrectionError*Magnitude);
          RightCorrection = RightOffset - (CorrectionError*Magnitude);
        }
        else
        {
          LeftCorrection = LeftOffset;
          RightCorrection = RightOffset;
        }

        L.spin(fwd, (ToVolt(Power)*LeftCorrection), volt);
        R.spin(fwd, (ToVolt(Power)*RightCorrection), volt);

        PDist = Dist;
      }
      else
      {
        StartInt = 15;
        Dist = Goal - trueHeading();
        if (trueHeading() >= (Dist + 180))
        {
          Dist = (Goal - trueHeading())+360;
        }
        if (Dist > 180)
        {
          Dist = Dist - 360;
        }

        if (fabs(Dist) < StartInt)
        {
          DistInt+=Dist;
        }
        if ((Dist>0 && PDist<0)||(Dist<0 && PDist>0))
        {
          DistInt = 0;
        }

        Power = (kp*Dist) + ki*DistInt + kd*(Dist-PDist);

        L.spin(fwd, (ToVolt(Power))*LeftOffset, volt);
        R.spin(reverse, (ToVolt(Power))*RightOffset, volt);

        PDist = Dist;
      }
      if (!PidLock) { wait(10, msec); }         // wait only if the lock is still off, otherwise go straight to stopping motors
    }
    else
    {
      L.stop(hold);
      R.stop(hold);
      //waitUntil(!PidLock);
    }
  }
  return 1;
}

std::string overunder;
std::string settletimeout;
// gives "accurate" turns with some oscillation? idk
//void Turn(float Heading, float Timeout = 2, float KKP = -1, float KKI = 0.45, float KKD = 1.45, float LP = 1, float RP = 1)
// new with no oscillation (bad ish)


void SwerveR(float Heading, float Timeout, float KKP, float KKI, float KKD, float SL, float ST)
{
  L.stop(brake);
  R.stop(brake);
  float tError = 0;
  //VTracker.setPosition(0,degrees);
  tError = Heading - trueHeading();
  if (trueHeading() >= (tError + 180)) { tError = (Heading - trueHeading())+360; }
  if (tError > 180) { tError -= 360; }

  //kp = (KKP == -1) ? -0.00560613 * fabs(error) + 2.31934 : KKP;
  kp = KKP;
  ki = KKI;
  kd = KKD;
  LeftOffset = 0;
  RightOffset = 1;
  Goal = Heading;
  
  DistInt = 0;
  PDist = 0;
  Power = 0;

  CorrectLock = true;
  PidLock = false;
  Linear = false;
  TSLimit = SL;
  TSThreshold = ST;
  TimerTurn.reset();
  TurnTime.reset();
  task pid(PID);
  task settleT(TurnSettle);
  waitUntil(TurnTime.value() >= Timeout || Settled);



  PidLock = true;
  task::stop(settleT);
  task::stop(pid);
  L.stop(hold);
  R.stop(hold);
  overunder = (Dist<=0) ? " over\t " : " under\t ";
  settletimeout = (Settled) ? "   Settled in " : "   Timed out in ";
  std::cout << "(SwerveR)     Goal: " << Heading << "\t    Actual: " << trueHeading() << "\t Error: " << fabs(Dist) << overunder << settletimeout << TurnTime.value() << " sec \n";
}


void SwerveL(float Heading, float Timeout, float KKP, float KKI, float KKD, float SL, float ST)
{
  L.stop(brake);
  R.stop(brake);
  float tError = 0;
  //VTracker.setPosition(0,degrees);
  tError = Heading - trueHeading();
  if (trueHeading() >= (tError + 180)) { tError = (Heading - trueHeading())+360; }
  if (tError > 180) { tError -= 360; }

  //kp = (KKP == -1) ? -0.00560613 * fabs(error) + 2.31934 : KKP;
  kp = KKP;
  ki = KKI;
  kd = KKD;
  LeftOffset = 1;
  RightOffset = 0;
  Goal = Heading;
  
  DistInt = 0;
  PDist = 0;
  Power = 0;

  CorrectLock = true;
  PidLock = false;
  Linear = false;
  TSLimit = SL;
  TSThreshold = ST;
  TimerTurn.reset();
  TurnTime.reset();
  task pid(PID);
  task settleT(TurnSettle);
  waitUntil(TurnTime.value() >= Timeout || Settled);



  PidLock = true;
  task::stop(settleT);
  task::stop(pid);
  L.stop(hold);
  R.stop(hold);
  overunder = (Dist<=0) ? " over\t " : " under\t ";
  settletimeout = (Settled) ? "   Settled in " : "   Timed out in ";
  std::cout << "(SwerveL)     Goal: " << Heading << "\t    Actual: " << trueHeading() << "\t Error: " << fabs(Dist) << overunder << settletimeout << TurnTime.value() << " sec \n";
}


void Turn(float Heading, float Timeout, float KKP, float KKI, float KKD, float SL, float ST)
{
  L.stop(brake);
  R.stop(brake);
  float tError = 0;
  //VTracker.setPosition(0,degrees);
  tError = Heading - trueHeading();
  if (trueHeading() >= (tError + 180)) { tError = (Heading - trueHeading())+360; }
  if (tError > 180) { tError -= 360; }

  //kp = (KKP == -1) ? -0.00560613 * fabs(error) + 2.31934 : KKP;
  kp = KKP;
  ki = KKI;
  kd = KKD;
  LeftOffset = 1;
  RightOffset = 1;
  Goal = Heading;
  
  DistInt = 0;
  PDist = 0;
  Power = 0;

  CorrectLock = true;
  PidLock = false;
  Linear = false;
  TSLimit = SL;
  TSThreshold = ST;
  TimerTurn.reset();
  TurnTime.reset();
  task pid(PID);
  task settleT(TurnSettle);
  waitUntil(TurnTime.value() >= Timeout || Settled);



  PidLock = true;
  task::stop(settleT);
  task::stop(pid);
  L.stop(hold);
  R.stop(hold);
  overunder = (Dist<=0) ? " over\t " : " under\t ";
  settletimeout = (Settled) ? "   Settled in " : "   Timed out in ";
  std::cout << "(Turn)     Goal: " << Heading << "\t    Actual: " << trueHeading() << "\t Error: " << fabs(Dist) << overunder << settletimeout << TurnTime.value() << " sec \n";
}

void TurnS(float Heading, float Timeout, float SL, float ST, float KKP)
{
  CorrectLock = true;
  Turn(Heading, Timeout, KKP, 0.12, 16, SL, ST);
}

float AngleError = 0;

int HeadingHold(float Angle)
{
  AngleError = Angle - trueHeading();
  return 1;
}

void Drive(float Distance, float Timeout, float KKP, float KKI, float KKD, float SL, float ST)
{
  L.stop(brake);
  R.stop(brake);
  VTracker.setPosition(0, turns);
  kp = KKP;
  ki = KKI;
  kd = (KKD == -1) ? 0.7 + ((fabs(Distance) - 600)/12000) : KKD;

  LeftOffset = 1;
  RightOffset = 1;
  Magnitude = 0;
  AutoAngle = 0;
  Goal = ToMM(Distance);
  
  DistInt = 0;
  PDist = 0;
  Power = 0;

  CorrectLock = true;
  PidLock = false;
  Linear = true;
  DSLimit = (SL == -1) ? fabs(Distance / 60) : SL;

  DSThreshold = ST;
  TimerDrive.reset();
  DriveTime.reset();
  task pid(PID);
  task settleD(DriveSettle);
  waitUntil(DriveTime.value() >= Timeout || Settled);

  PidLock = true;
  task::stop(settleD);
  task::stop(pid);
  L.stop(hold);
  R.stop(hold);
  overunder = (Dist<=0) ? " over\t " : " under\t ";
  settletimeout = (Settled) ? "   Settled in " : "   Timed out in ";
  std::cout << "(Drive)    Goal: " << Distance << "\t    Actual: " << VTracker.position(turns)*M_PI*TrackerWheelSize << "\t Error: " << fabs(Dist) << overunder << settletimeout << DriveTime.value() << " sec \n";

}

void DriveS(float Distance, float Timeout, float SL, float ST, float KKP, float KKI, float KKD, float DNSB)
{
  CorrectLock = true;
  DNoSettleBefore = DNSB;
  Drive(Distance, Timeout, KKP, KKI, KKD, SL, ST);
  DNoSettleBefore = 0;
}

void Arc(float Distance, float Timeout, float LP, float RP, float KKP, float KKI, float KKD)
{
  CorrectLock = true;
  LeftOffset = LP;
  RightOffset = RP;
  Drive(Distance, Timeout, KKP, KKI, KKD);
  //CorrectLock = false;
}

/**
int Align()
{
  PidOn = true;
  while (PidOn)
  {
    if (!AlignLock)
    {
      FrontVision.takeSnapshot(aivision::aiobjdesc(AColor));    // red ring has id 1, blue ring has id 2, and the enum attaches the same numbers to the objects
      CenterDist = FrontVision.objects[0].centerX - 160;
      if (FrontVision.objects[0].exists && FrontVision.objects[0].width >= MinWidth)
      {
        APower = CenterDist * akp;
        L.spin(forward, ToVolt(APower), volt);
        R.spin(reverse, ToVolt(APower), volt);
      }
      wait(10, msec);
    }
    else
    {
      L.stop(brake);
      R.stop(brake);
      waitUntil(!AlignLock);
    }
  }
  return 1;
}

void FrontAlign(int clr, float Timeout = 1, float KP = 0.5, float mwidth = 15)
{
  VTracker.setPosition(0, degrees);
  akp = KP;
  AColor = clr;
  MinWidth = mwidth;
  AlignLock = false;
  task align(Align);
  wait(Timeout, sec);
  AlignLock = true;
  wait(20, msec);
  task::stop(align);
}
/**/
