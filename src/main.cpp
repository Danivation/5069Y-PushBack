#include "vex.h"

using namespace vex;
competition Competition;
odom Odom;
std::string selectedAutonName = "";

int CScreen()
{
  while (true)
  {
    Controller.Screen.clearScreen();

    /**/
    Controller.Screen.setCursor(1, 1);
    Controller.Screen.print("I: %3.3f (%1.3f)", trueHeading(), toRadians(trueHeading()));
    Controller.Screen.setCursor(2, 1);
    Controller.Screen.print("LB: %1.2f", LBRotation.position(turns));
    /**
    Controller.Screen.setCursor(1, 1);
    //Controller.Screen.print("L: %.3f", (LA.velocity(rpm)+LB.velocity(rpm)+LB.velocity(rpm))/3);
    Controller.Screen.print("L: %.3f", L.velocity(rpm));
    Controller.Screen.setCursor(2, 1);
    Controller.Screen.print("R: %.3f", R.velocity(rpm));
    /**/
    Controller.Screen.setCursor(3, 1);
    Controller.Screen.print("B: %d%%", Brain.Battery.capacity(percent));

    wait(50, msec);
  }
}

int BScreen()
{
  //Brain.Screen.clearScreen();
  while (true)
  {
    //Brain.Screen.printAt(0, 60, true, "true vertical: %.2f", VTracker.position(degrees) * (M_PI * 2) / 360);
    //Brain.Screen.printAt(0, 80, true, "prev v: %f", VPrevious);

    Odom.currentOdomPose.x = Odom.X_position;
    Odom.currentOdomPose.y = Odom.Y_position;
    //Brain.Screen.printAt(0, 100, true, "global x: %.2f", Odom.X_position);
    //Brain.Screen.printAt(0, 120, true, "global y: %.2f", Odom.Y_position);
    //Brain.Screen.printAt(0, 140, true, "global theta: %.2f", trueHeading());


    //Brain.Screen.printAt(0, 180, true, "L: %.3f", (LA.velocity(rpm)+LB.velocity(rpm)+LB.velocity(rpm))/3);
    //Brain.Screen.printAt(0, 200, true, "R: %.3f", (RA.velocity(rpm)+RB.velocity(rpm)+RB.velocity(rpm))/3);


    wait(50, msec);
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
  L.stop(coast);
  R.stop(coast);
  L.setStopping(coast);
  R.setStopping(coast);
  ColorLock = true;
  CorrectLock = true;
  AJEnable = false;

  //task dBScreen             (BScreen);
  task dCScreen             (CScreen);

  task dDrivetrainControl   (DrivetrainControl);
  task dConveyorControl     (ConveyorControl);
  task dClampToggleControl  (ClampToggleControl);
  task dLBReset             (LBReset);
  task dLBControl           (LBControl);
  task dLBLoadControl       (LBLoadControl);
  task dLBDescoreControl    (LBDescore2Control);
  task dLBDescore2Control   (LBDescoreControl);
  task dDoinkerLControl     (DoinkerLControl);
  task dDoinkerRControl     (DoinkerRControl);
}

void auton()
{
  // wait for field control or competition switch
  waitUntil((Competition.isFieldControl() || Competition.isCompetitionSwitch()) && Competition.isEnabled());

  std::cout << "AUTON ACTIVE \n";

  CorrectLock = true;
  task aLBReset(LBReset);
  //task aBScreen(BScreen);
  task aCScreen(CScreen);
  task aColorSort(ColorSort);
  task aAntiJam(jamdetect);

  //wait(1.5, sec);
  switch (selectedAuton + autonMode)
  {
    // 1, 4, 7, 10: basic win points
    // 2, 8: swoop swp
    // 5, 11: none
    // 3, 9: swoop elims
    // 6, 12: rush elims

    // 1-3: blue ring side
    // 4-6: blue goal side
    // 7-9: red ring side
    // 10-12: red goal side
    default:
      autonNone();
    case 202: // none
      //autonNone();
      break;
    case 1: // blue ring basic win point
      autonBlueRingBasicWP();
      break;
    case 2: // blue ring swoop swp
      autonBlueRingSwoopSWP();
      break;
    case 3: // blue ring swoop elims
      autonBlueRingSwoopElims();
      break;
    case 4: // blue goal basic win point
      autonBlueGoalBasicWP();
      break;
    case 5: // blue goal (RING SIDE swoop corner wp)
      autonBlueRingSwoopCornerWP();
      break;
    case 6: // blue goal rush elims
      autonBlueGoalRushElims();
      break;
    case 7: // red ring basic win point
      autonRedRingBasicWP();
      break;
    case 8: // red ring swoop swp
      autonRedRingSwoopSWP();
      break;
    case 9: // red ring swoop elims
      autonRedRingSwoopElims();
      break;
    case 10: // red goal basic win point
      autonRedGoalBasicWP();
      break;
    case 11: // red goal (RING SIDE swoop corner wp)
      autonRedRingSwoopCornerWP();
      break;
    case 12: // red goal rush elims
      autonRedGoalRushElims();
      break;
    case 201: // skills
      autonSkills();
      break;
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
  Clamp.set(false);
  DoinkerL.set(false);
  DoinkerR.set(false);
  ConveyorMotor.setStopping(coast);
  ConveyorMotor.setVelocity(600, rpm);
  LBMotorL.setStopping(hold);
  LBMotorL.setVelocity(200, rpm);
  LBMotorR.setStopping(hold);
  LBMotorR.setVelocity(200, rpm);
  HTracker.resetPosition();
  VTracker.resetPosition();
  RingColor.setLightPower(100, percent);
  RingColor.setLight(ledState::on);
  RingColor.integrationTime(2);
  RingColor.objectDetectThreshold(150);
  FrontVision.modelDetection(true);

  // make sure nothing is unplugged
  /**/
  if ( !VTracker.installed()
    || !ConveyorMotor.installed()
    || !Inertial1.installed()
    || !Inertial2.installed()
    || !LA.installed()
    || !LB.installed()
    || !LC.installed()
    || !RA.installed()
    || !RB.installed()
    || !RC.installed()
    || !LBMotorL.installed()
    || !LBMotorR.installed()
    || !LBRotation.installed()
    || !RingColor.installed()
    || !RingDistance.installed()
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
  task mCScreen(CScreen);

  while (true)
  {
    wait(50, msec);
  }
  return 1;
}
