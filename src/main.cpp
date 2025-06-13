#include "vex.h"

using namespace vex;
competition Competition;
std::string selectedAutonName = "";

int CScreen()
{
  while (true)
  {
    Controller.Screen.clearScreen();

    Controller.Screen.setCursor(1, 1);
    Controller.Screen.print("I: %3.3f (%1.3f)          ", trueHeading(), toRadians(trueHeading()));
    Controller.Screen.setCursor(2, 1);
    //Controller.Screen.print("LB: %1.2f            ", LBRotation.position(turns));
    Controller.Screen.setCursor(3, 1);
    Controller.Screen.print("B: %d%%            ", Brain.Battery.capacity(percent));

    wait(150, msec);
  }
}

int BScreen()
{
  //Brain.Screen.clearScreen();
  while (true)
  {
    // print things
    Brain.Screen.printAt(0, 40, "L: %1.5f   ", ((Inertial1.rotation(degrees)/360.0f) / std::round(Inertial1.rotation(degrees)/360.0f)));
    Brain.Screen.printAt(0, 60, "R: %1.5f   ", ((Inertial2.rotation(degrees)/360.0f) / std::round(Inertial2.rotation(degrees)/360.0f)));
    VecPose pose = Odometry.getPose();
    Brain.Screen.printAt(0, 100, "x: %2.2f, y: %2.2f, heading: %2.2f             ", pose.x, pose.y, pose.theta);

    wait(100, msec);
  }
}

void driver()
{
  // wait for field control or competition switch
  waitUntil(((Competition.isFieldControl() || Competition.isCompetitionSwitch()) && Competition.isEnabled()) || Bypass || (Controller.ButtonUp.pressing() && Controller.ButtonX.pressing()));
  wait(10, msec);

  std::cout << "DRIVER ACTIVE \n";
  Brain.Screen.clearScreen();

  task::stop(PID);
  L.stop(hold);
  R.stop(hold);
  L.setStopping(coast);
  R.setStopping(coast);
  ColorLock = true;
  CorrectLock = true;

  vex::task odomTask = launch_task(std::bind(&odometry::startTracking, &Odometry, 0, 0, 0));

  task dCScreen             (CScreen);
  task dBScreen             (BScreen);

  task dDrivetrainControl   (DrivetrainControl);
  task dIntakeControl       (IntakeControl);
  task dLiftControl         (LiftControl);
  task dLoaderControl       (LoaderControl);
  task dStopperControl      (StopperControl);
  task dTrapdoorControl     (TrapdoorControl);
}

void auton()
{
  // wait for field control or competition switch
  waitUntil((Competition.isFieldControl() || Competition.isCompetitionSwitch()) && Competition.isEnabled());

  std::cout << "AUTON ACTIVE \n";

  CorrectLock = true;
  task aCScreen(CScreen);
  //task aColorSort(ColorSort);

  //wait(1.5, sec);
  switch (selectedAuton + autonMode)
  {
    default:
      autonNone();
  }
  Brain.Screen.clearScreen();
  while (true)
  {
    wait(50, msec);
  }
}

int main()
{
  // initialize things i guess
  vexcodeInit();
  Competition.autonomous(auton);
  Competition.drivercontrol(driver);
  competition::bStopAllTasksBetweenModes = true;
  std::cout << "INIT DONE \n";

  // setup things 
  L.setStopping(coast);
  R.setStopping(coast);
  Lift.set(false);
  Intake.setStopping(hold);
  Intake.setVelocity(600, rpm);
  BlockColor.integrationTime(2);

  // make sure nothing is unplugged
  /**
  if ( !VTracker.installed()
    || !HTracker.installed()
    || !Inertial1.installed()
    || !Inertial2.installed()
    || !LA.installed()
    || !LB.installed()
    || !LC.installed()
    || !RA.installed()
    || !RB.installed()
    || !RC.installed()
  ) {Brain.programStop();}
  /**/
  
  // set up auton selector
  task mAutonSelect(AutonSelect);
  std::cout << "SETUP DONE \n";

  Controller.Screen.clearScreen();
  wait(200, msec);
  task mCScreen(CScreen);

  // calibrate inertial
  Inertial1.calibrate();
  Inertial2.calibrate();
  wait(3, sec);
  setInertial(0);
  Controller.rumble(rumbleShort);
  std::cout << "CALIBRATE DONE \n";

  // wait until control is plugged in or bypass is enabled
  waitUntil(((Competition.isFieldControl() || Competition.isCompetitionSwitch()) && !Competition.isEnabled()) || Bypass || (Controller.ButtonY.pressing() && Controller.ButtonRight.pressing()));
  mCScreen.stop();
  Controller.Screen.clearScreen();
  wait(200, msec);
  task mCScreen2(CScreen);

  while (true)
  {
    wait(50, msec);
  }
  return 1;
}
