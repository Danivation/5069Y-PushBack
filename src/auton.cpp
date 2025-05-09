#include "vex.h"

using namespace vex;
extern odom Odom;

void setInertial(float angle)
{
  Inertial1.setHeading(angle, degrees);
  Inertial2.setHeading(angle, degrees);
  Inertial1.setRotation(angle, degrees);
  Inertial2.setRotation(angle, degrees);
}

timer stopTime;
int stoptimer()
{
  stopTime.reset();
  waitUntil(stopTime.value() >= 15.0f);
  
  L.stop(hold);
  R.stop(hold);
  ConveyorMotor.stop(hold);
  LBMotors.stop(hold);
  printf("final time: %f\n", Brain.Timer.value());
  task::stopAll();
  return 1;
}

int lbforward()
{
  wait(waitTime, sec);
  LBMotors.spin(forward, 12, volt);
  return 1;
}

float scoreKP = 140;
float scorePos = 2.75;

float holdPos = 2.6;

int holdpos()
{
  wait(waitTime, sec);
  float error;
  float kp = scoreKP;
  while (true)
  {
    error = (holdPos - LBRotation.position(turns));  
    //Brain.Screen.printAt(0, 100, "%f", error);
    //Brain.Screen.printAt(0, 150, "%f", error*kp);
    LBMotorL.spin(forward, ToVolt(error*kp), volt); 
    LBMotorR.spin(forward, ToVolt(error*kp), volt); 
    wait(15, msec);
  }
  return 1;
}

int alliancescore()
{
  wait(waitTime, sec);
  float error;
  float kp = scoreKP;
  while (true)
  {
    error = (scorePos - LBRotation.position(turns));  
    //Brain.Screen.printAt(0, 100, "%f", error);
    //Brain.Screen.printAt(0, 150, "%f", error*kp);
    LBMotorL.spin(forward, ToVolt(error*kp), volt); 
    LBMotorR.spin(forward, ToVolt(error*kp), volt); 
    wait(15, msec);
  }
  return 1;
}

float raisePos = 1.5;
int lbraise()
{
  wait(waitTime, sec);
  float error;
  float kp = scoreKP;
  while (true)
  {
    error = (raisePos - LBRotation.position(turns));  
    //Brain.Screen.printAt(0, 100, "%f", error);
    //Brain.Screen.printAt(0, 150, "%f", error*kp);
    LBMotorL.spin(forward, ToVolt(error*kp), volt); 
    LBMotorR.spin(forward, ToVolt(error*kp), volt); 
    wait(15, msec);
  }
  return 1;
}

float wallPos = 2.2;
int lbwall()
{
  wait(waitTime, sec);
  float error;
  float kp = scoreKP;
  while (true)
  {
    error = (wallPos - LBRotation.position(turns));  
    //Brain.Screen.printAt(0, 100, "%f", error);
    //Brain.Screen.printAt(0, 150, "%f", error*kp);
    LBMotorL.spin(forward, ToVolt(error*kp), volt); 
    LBMotorR.spin(forward, ToVolt(error*kp), volt); 
    wait(15, msec);
  }
  return 1;
}

int ldoinkerd()
{
  wait(waitTime, sec);
  DoinkerL.set(true);
  return 1;
}

int rdoinkerd()
{
  wait(waitTime, sec);
  DoinkerR.set(false);
  return 1;
}




// Auton functions
void autonNone()
{
  /**
  ColorLock = false;
  AJEnable = false;
  SortColor = &blue;
  ConveyorMotor.spin(forward, 12, volt);
  /**/

  
  task aStop(stoptimer);
  autonRedRingSwoopElims();
  printf("final time: %f\n", Brain.Timer.value());
}

void cornerGrab()
{
  Drive(-200, 0.1);
  Drive(200, 0.7);
  Drive(-400, 0.7, 0.095);
}

//
void autonRedGoalBasicWP()
{
  // BASIC WP red goal side
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = false;
  LBRotation.setPosition(1.33, turns);

  setInertial(360-116.8);

  LBMotors.spin(forward, 12, volt);
  wait(0.6, sec);
  LBMotors.stop(hold);
  Drive(-150, 0.4);

  LBMotors.spin(reverse, 12, volt);
  Turn(287);
  Drive(210);
  DoinkerR.set(true);
  wait(0.1, sec);
  Turn(213);
  Drive(-930);
  Clamp.set(true);
  AJEnable = true;
  ColorLock = false;

  DoinkerR.set(false);
  Turn(220);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(250);
  Turn(332);
  ConveyorMotor.stop(coast);
  task stop1(ringstop2);
  task stop2(ringstop);
  Drive(580);
  DoinkerR.set(true);



  /**
  Clamp.set(true);
  AJEnable = true;
  ColorLock = false;
  ConveyorMotor.spin(forward, 12, volt);
  waitTime = 0.5;
  task undoink1(rdoinker);
  Turn(87);
  Drive(700, 1.1);
  wait(0.3, sec);
  DoinkerR.set(false);
  Turn(165);
  
  waitTime = 0.5;
  task conveyorback1(conveyorback);
  task hold1(holdpos);
  Drive(1050, 1.3);
  task::stop(holdpos);
  ConveyorMotor.spin(forward);
  Drive(-200, 0.1);
  Drive(200, 0.4);            // corner
  Drive(-400, 0.7, 0.095);
  LBMotors.spin(reverse, 12, volt);
  DoinkerR.set(true);
  Turn(312);
  DoinkerR.set(false);
  waitTime = 0.6;
  task conveyorstop1(cstop);
  /**
  Drive(1220, 1.2);
  Turn(315);
  Drive(240);
  DoinkerR.set(true);
  wait(0.1, sec);
  Drive(-600);
  DoinkerR.set(false);
  Turn(340);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(200);
  /**/
}

void autonBlueGoalBasicWP()
{
  // MIRRORED
  // BASIC WP blue goal side
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = false;
  AJEnable = false;
  LBRotation.setPosition(1.33, turns);

  setInertial(116.8);

  task score1(alliancescore);
  wait(0.6, sec);
  task::stop(score1);
  LBMotors.stop(hold);
  Drive(-150, 0.4);

  LBMotors.spin(reverse, 12, volt);
  SwerveR(360-310);
  Drive(100, 0.3);
  DoinkerL.set(true);
  wait(0.1, sec);
  Turn(360-213);
  Drive(-930);
  Clamp.set(true);
  AJEnable = true;
  ColorLock = false;
  ConveyorMotor.spin(forward, 12, volt);
  waitTime = 0.5;
  task undoink1(ldoinker);
  Turn(360-87);
  Drive(700, 1.1);
  wait(0.3, sec);
  DoinkerL.set(false);
  Turn(360-165);
  
  waitTime = 0.5;
  task conveyorback1(conveyorback);
  task hold1(holdpos);
  Drive(1050, 1.3);
  task::stop(holdpos);
  ConveyorMotor.spin(forward);
  Drive(-200, 0.1);
  Drive(200, 0.4);            // corner
  Drive(-400, 0.7, 0.095);
  LBMotors.spin(reverse, 12, volt);
  DoinkerL.set(true);
  Turn(360-312);
  DoinkerL.set(false);
  waitTime = 0.6;
  task conveyorstop1(cstop);
  Drive(1220, 1.2);
  /**
  Turn(360-315);
  Drive(240);
  DoinkerL.set(true);
  wait(0.1, sec);
  Drive(-600);
  DoinkerL.set(false);
  Turn(360-340);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(200);
  /**/
  LBMotors.spin(forward, 12, volt);
}

void autonRedRingBasicWP()
{
  // MIRRORED
  // BASIC WP red ring side
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = false;
  LBRotation.setPosition(1.33, turns);

  setInertial(116.8);

  task score1(alliancescore);
  wait(0.6, sec);
  task::stop(score1);
  LBMotors.stop(hold);
  Drive(-150, 0.4);

  LBMotors.spin(reverse, 12, volt);
  SwerveR(360-310);
  Drive(100, 0.3);
  DoinkerL.set(true);
  wait(0.1, sec);
  Turn(360-213);
  Drive(-930);
  Clamp.set(true);
  wait(0.1, sec);
  AJEnable = true;
  ColorLock = false;
  ConveyorMotor.spin(forward, 12, volt);
  waitTime = 0.5;
  task undoink1(ldoinker);
  Turn(360-87);
  Drive(700, 1.1);
  wait(0.3, sec);
  DoinkerL.set(false);
  Turn(360-165);
  
  waitTime = 0.5;
  task conveyorback1(conveyorback);
  task hold1(holdpos);
  Drive(1050, 1.3);
  task::stop(holdpos);
  ConveyorMotor.spin(forward);
  Drive(-200, 0.1);
  Drive(200, 0.4);            // corner
  Drive(-400, 0.7, 0.095);
  LBMotors.spin(reverse, 12, volt);
  DoinkerL.set(true);
  Turn(360-312);
  DoinkerL.set(false);
  waitTime = 0.6;
  task conveyorstop1(cstop);
  Drive(1220, 1.2);
  /**
  Turn(360-317);
  Drive(240);
  DoinkerL.set(true);
  wait(0.3, sec);
  Drive(-600);
  DoinkerL.set(false);
  Turn(360-340);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(200, 0.6);
  /**/ 
  LBMotors.spin(forward, 12, volt);
}

void autonBlueRingBasicWP()
{
  // BASIC WP blue ring side
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = false;
  AJEnable = false;
  LBRotation.setPosition(1.33, turns);

  setInertial(360-116.8);

  task score1(alliancescore);
  wait(0.6, sec);
  task::stop(score1);
  LBMotors.stop(hold);
  Drive(-150, 0.4);

  LBMotors.spin(reverse, 12, volt);
  SwerveL(310);
  Drive(100, 0.3);
  DoinkerR.set(true);
  wait(0.1, sec);
  Turn(213);
  Drive(-930);
  Clamp.set(true);
  AJEnable = true;
  ColorLock = false;
  ConveyorMotor.spin(forward, 12, volt);
  waitTime = 0.5;
  task undoink1(rdoinker);
  Turn(87);
  Drive(700, 1.1);
  wait(0.3, sec);
  DoinkerR.set(false);
  Turn(165);
  
  waitTime = 0.5;
  task conveyorback1(conveyorback);
  task hold1(holdpos);
  Drive(1050, 1.3);
  task::stop(holdpos);
  ConveyorMotor.spin(forward);
  Drive(-200, 0.1);
  Drive(200, 0.4);            // corner
  Drive(-400, 0.7, 0.095);
  LBMotors.spin(reverse, 12, volt);
  DoinkerR.set(true);
  Turn(312);
  DoinkerR.set(false);
  waitTime = 0.6;
  task conveyorstop1(cstop);
  Drive(1220, 1.2);
  /**
  Turn(315);
  Drive(240);
  DoinkerR.set(true);
  wait(0.1, sec);
  Drive(-600);
  DoinkerR.set(false);
  Turn(340);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(200);
  /**/
  LBMotors.spin(forward, 12, volt);
}

void autonRedRingSwoopSWP()
{
  // RED RING SWOOP SOLO WP
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = false;


  setInertial(116.8);

  LBMotors.spin(forward, 12, volt);
  wait(0.6, sec);
  LBMotors.stop(hold);
  //LBMotors.spin(reverse, 12, volt);
  //wait(0.1, sec);
  Drive(-100, 0.4);
  LBMotors.spin(reverse, 12, volt);

  SwerveR(166, 0.85);

  Drive(-800, 0.8);
  LBMotors.stop();
  waitTime = 0.3;
  task clamp1(mogo);
  Drive(-240, 0.6);
  Turn(303, 0.7);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveR(270, 0.7);
  AJEnable = true;
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-450, 0.8);
  SwerveR(175, 0.7);  // side ring swoop
  Drive(100, 0.4);


  //AJEnable = false;
  Turn(135);
  //DoinkerR.set(true);
  Drive(1000, 1.3);   // drive to alliance stake ring
  AJEnable = false;
  Clamp.set(false);
  Turn(90);
  ColorLock = true;

  Drive(980, 1.5, 0.08);    // thryu ring
  task stop1(ringstop2);
  task stop2(ringstop);
  Turn(180);
  task raise1(lbraise);
  Drive(-600, 0.9);
  Clamp.set(true);
  wait(0.3, sec);
  ConveyorMotor.spin(forward, 12, volt);
  SwerveL(125, 0.6);

  ConveyorMotor.stop(coast);
  Drive(-300, 0.6);
  task::stop(raise1);

  LBMotors.spin(reverse, 12, volt);
  printf("final time: %f\n", Brain.Timer.value());
}

void autonBlueRingSwoopSWP()
{
  // MIRRORED
  // BLUE RING SWOOP SOLO WP
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = false;
  AJEnable = false;


  setInertial(360-116.8);

  LBMotors.spin(forward, 12, volt);
  wait(0.6, sec);
  LBMotors.stop(hold);
  //LBMotors.spin(reverse, 12, volt);
  //wait(0.1, sec);
  Drive(-100, 0.4);
  LBMotors.spin(reverse, 12, volt);

  SwerveL(360-166, 0.85);

  Drive(-800, 0.8);
  LBMotors.stop();
  waitTime = 0.3;
  task clamp1(mogo);
  Drive(-240, 0.6);
  Turn(360-303, 0.7);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveR(270, 0.7);
  AJEnable = true;
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-450, 0.8);
  SwerveL(360-175, 0.7);  // side ring swoop
  Drive(100, 0.4);


  //AJEnable = false;
  Turn(360-135);
  //DoinkerR.set(true);
  Drive(1000, 1.3);   // drive to alliance stake ring
  AJEnable = false;
  Clamp.set(false);
  Turn(360-90);
  ColorLock = true;

  Drive(980, 1.5, 0.08);    // thryu ring
  task stop1(ringstop2);
  task stop2(ringstop);
  Turn(180);
  task raise1(lbraise);
  Drive(-600, 0.9);
  Clamp.set(true);
  wait(0.3, sec);
  ConveyorMotor.spin(forward, 12, volt);
  SwerveR(360-125, 0.6);

  ConveyorMotor.stop(coast);
  Drive(-300, 0.6);
  task::stop(raise1);

  LBMotors.spin(reverse, 12, volt);
  printf("final time: %f\n", Brain.Timer.value());
}

void autonRedRingSwoopCornerWP()
{
  // MIRRORED
  // RED CORNER SWOOP WP
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = true;
  //task aStop(stoptimer);


  setInertial(116.8);

  LBMotors.spin(forward, 12, volt);
  wait(0.6, sec);
  LBMotors.stop(hold);
  //LBMotors.spin(reverse, 12, volt);
  //wait(0.1, sec);
  Drive(-100, 0.4);
  LBMotors.spin(reverse, 12, volt);

  SwerveR(166, 0.85);

  Drive(-800, 0.8);
  LBMotors.stop();
  waitTime = 0.2;
  task clamp(mogo);
  Drive(-240, 0.6);
  Turn(303, 0.6);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveR(270, 0.7);
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-440, 0.8);
  AJEnable = true;
  SwerveR(175, 0.7);  // side ring swoop
  Drive(150, 0.7);
  //Turn(85);
  //Drive(200);
  Turn(190, 0.6);    // corner
  
  waitTime = 0.5;
  //task conveyorback1(conveyorback);
  
  task hold1(holdpos);
  Drive(970, 0.8);
  Turn(215, 0.3);
  AJEnable = false;
  Drive(300, 0.3);
  ConveyorMotor.spin(forward);  // corner
  Drive(-200, 0.1);
  Drive(200, 0.7);
  Drive(-400, 0.7, 0.095);
  task::stop(hold1);
  LBMotors.spin(reverse, 12, volt);
  AJEnable = true;
  Turn(360-315);
  waitTime = 0.5;
  task cstop2(cstop);
  Drive(1050);  // ladder
  LBMotors.spin(forward, 12, volt);
  printf("final time: %f\n", Brain.Timer.value());
}

void NEW_autonRedRingSwoopCornerWP()
{
  // MIRRORED
  // RED CORNER SWOOP WP
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = true;
  //task aStop(stoptimer);


  setInertial(180-(360-329.7));
  
  /**/
  LBMotors.spin(forward, 12, volt);
  wait(0.7, sec);
  LBMotors.spin(reverse, 12, volt);
  waitTime = 0.9;
  task clamp(mogo);
  Drive(-1000, 1.2);
  Turn(303, 0.6);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveR(270, 0.7);
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-440, 0.8);
  AJEnable = true;
  SwerveR(175, 0.7);  // side ring swoop
  Drive(150, 0.7);
  //Turn(85);
  //Drive(200);
  Turn(190, 0.6);    // corner
  
  waitTime = 0.5;
  //task conveyorback1(conveyorback);
  
  task hold1(holdpos);
  Drive(970, 0.8);
  Turn(215, 0.3);
  AJEnable = false;
  Drive(300, 0.3);
  ConveyorMotor.spin(forward, 12, volt);  // corner
  Drive(-200, 0.1);
  Drive(200, 0.7);
  Drive(-400, 0.7, 0.095);
  task::stop(hold1);

  Turn(85);
  LBMotors.spin(reverse, 12, volt);
  waitTime = 0.5;
  task cout1(conveyorback);
  Drive(1750, 1.5);
  LBMotors.stop();
  SwerveR(315);
  Drive(150);
  LBMotors.spin(forward, 12, volt);



  printf("final time: %f\n", Brain.Timer.value());
}

void autonBlueRingSwoopCornerWP()
{
  // BLUE CORNER SWOOP WP
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = false;
  AJEnable = false;
  //task aStop(stoptimer);


  setInertial(360-116.8);

  LBMotors.spin(forward, 12, volt);
  wait(0.6, sec);
  LBMotors.stop(hold);
  //LBMotors.spin(reverse, 12, volt);
  //wait(0.1, sec);
  Drive(-100, 0.4);
  LBMotors.spin(reverse, 12, volt);

  SwerveL(360-166, 0.85);

  Drive(-800, 0.8);
  LBMotors.stop();
  waitTime = 0.2;
  task clamp(mogo);
  Drive(-240, 0.6);
  Turn(360-303, 0.6);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveL(360-270, 0.7);
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-440, 0.8);
  AJEnable = true;
  SwerveL(360-175, 0.7);  // side ring swoop
  Drive(150, 0.7);
  //Turn(85);
  //Drive(200);
  Turn(360-190, 0.6);    // corner
  
  waitTime = 0.5;
  //task conveyorback1(conveyorback);
  task hold1(holdpos);
  Drive(970, 0.8);
  Turn(360-215, 0.3);
  AJEnable = false;
  Drive(300, 0.3);
  ConveyorMotor.spin(forward);  // corner
  Drive(-200, 0.1);
  Drive(200, 0.7);
  Drive(-400, 0.7, 0.095);
  task::stop(hold1);
  LBMotors.spin(reverse, 12, volt);
  AJEnable = true;
  Turn(315);
  waitTime = 0.5;
  task cstop2(cstop);
  Drive(1100);  // ladder
  LBMotors.spin(forward, 12, volt);
  printf("final time: %f\n", Brain.Timer.value());
}

void autonRedRingSwoopElims()
{
  // RED RING ELIMS SWOOP
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = false;
  //task aStop(stoptimer);


  setInertial(116.8);

  LBMotors.spin(forward, 12, volt);
  wait(0.55, sec);
  LBMotors.stop(hold);
  Drive(-100, 0.3);
  LBMotors.spin(reverse, 12, volt);

  SwerveR(166, 0.8);

  LBMotors.stop();
  waitTime = 0.8;
  task clamp(mogo);
  Drive(-930, 1.2);
  Turn(303, 0.6);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveR(270, 0.7);
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-440, 0.8);
  AJEnable = true;
  SwerveR(175, 0.7);  // side ring swoop
  Drive(150, 0.7);
  //Turn(85);
  //Drive(200);
  Turn(190, 0.6);    // corner
  
  waitTime = 0.5;
  //task conveyorback1(conveyorback);
  
  task hold1(holdpos);
  Drive(970, 0.8);
  Turn(215, 0.3);
  AJEnable = false;
  Drive(300, 0.3);
  ConveyorMotor.spin(forward, 12, volt);  // corner
  Drive(-200, 0.1);
  Drive(200, 0.7);
  Drive(-400, 0.7, 0.095);
  LBMotors.spin(reverse, 12, volt);


  Turn(90, 0.8);
  ColorLock = false;
  AJEnable = true;
  ConveyorMotor.spin(forward, 12, volt);
  Drive(1050, 2.5);
  ConveyorMotor.spin(reverse, 12, volt);
  DoinkerL.set(true);
}

void autonBlueRingSwoopElims()
{
  // MIRRORED
  // BLUE RING ELIMS SWOOP
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = false;
  AJEnable = false;
  //task aStop(stoptimer);


  setInertial(360-116.8);

  task score1(alliancescore);
  wait(0.6, sec);
  task::stop(score1);
  LBMotors.stop(hold);
  Turn(360-108, 0.2, 2.3);
  DoinkerR.set(true);
  Drive(-100, 0.4);
  LBMotors.spin(reverse, 12, volt);

  SwerveL(360-166, 0.85);

  Drive(-800, 0.8);
  DoinkerR.set(false);
  LBMotors.stop();
  waitTime = 0.2;
  task clamp(mogo);
  Drive(-240, 0.6);
  Turn(360-303, 0.6);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(365, 0.65);
  SwerveL(360-270, 0.7);
  Drive(420); // swoop
  printf("swoop time: %f\n", Brain.Timer.value());//6.35,


  Drive(-440, 0.6);
  AJEnable = true;
  SwerveL(360-175, 0.7);  // side ring swoop
  Drive(100, 0.4);
  //Turn(85);
  //Drive(200);
  Turn(360-190, 0.6);    // corner
  
  waitTime = 0.5;
  task conveyorback1(conveyorback);
  Drive(970, 0.8);
  
  waitTime = 0;
  task hold1(holdpos);
  Turn(360-215, 0.3);
  Drive(300, 0.3);
  ConveyorMotor.spin(forward, 12, volt);  // corner
  Drive(-200, 0.1);
  Drive(200, 0.4);
  Drive(-400, 0.7, 0.095);
  LBMotors.spin(reverse, 12, volt);


  Turn(360-100, 0.8);
  ColorLock = false;
  AJEnable = true;
  DoinkerL.set(true);
  ConveyorMotor.spin(forward, 12, volt);
  waitTime = 0;
  task hold2(holdpos);
  Drive(3000, 2.5);
  Turn(360-315);
  //Turn(110, 0.5);
  //Drive(600);
  LBMotors.stop();
}

void autonRedGoalRushElims()
{
  // RED GOAL RUSH ELIMS
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = false;
  AJEnable = false;
  setInertial(0);
  LBRotation.setPosition(1.33, turns);

  waitTime = 0.5;
  task raise1(lbraise);
  Drive(950, 0.8);
  task::stop(raise1);

  SwerveL(40);
  L.stop(hold);
  R.stop(hold);
  waitTime = 0;
  task lbforward1(alliancescore);
  wait(0.5, sec);
  task::stop(lbforward1);
  waitTime = 0.2;
  task lbforward2(lbforward);
  //DoinkerR.set(true);
  Turn(40-120, 0.5);
  LBMotors.spin(forward, 12, volt);
  Turn(45);
  DoinkerR.set(false);
  Drive(-500);
  ConveyorMotor.spin(forward, 12, volt);
  wait(0.05, sec);
  ConveyorMotor.stop(coast);
  Clamp.set(true);
  LBMotors.spin(reverse, 12, volt);
  Turn(320);
  Drive(600);
  LBMotors.stop();
  DoinkerR.set(true);     // under ladder
  wait(0.1, sec);
  ConveyorMotor.spin(reverse, 12, volt);
  Drive(-1100, 1.5);
  AJEnable = true;
  
  waitTime = 0.3;
  task undoink1(rdoinker);
  Turn(30);

  ConveyorMotor.spin(forward, 12, volt);
  
  waitTime = 0;
  task load1(lbload);
  Drive(400);
  Turn(41);
  
  AJEnable = false;
  ColorLock = true;
  Drive(200, 0.5);


  /**
  Turn(230);
  Drive(1100);
  ConveyorMotor.stop(coast);



  task score1(alliancescore);
  wait(0.6, sec);
  task::stop(score1);
  LBMotors.stop(hold);
  Drive(-150, 0.4);

  LBMotors.spin(reverse, 12, volt);
  SwerveL(310);
  Drive(100, 0.3);
  DoinkerR.set(true);
  wait(0.1, sec);
  Turn(213);
  Drive(-930);
  /**/


  Turn(43);
  Drive(500);
  ConveyorMotor.stop(coast);
  waitTime = 0;
  task lbwall1(lbwall);
  wait(0.3, sec);
  ColorLock = false;
  ConveyorMotor.spin(forward, 12, volt);
}

void autonBlueGoalRushElims()
{
  // MIRRORED
  // BLUE GOAL RUSH ELIMS
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = false;
  AJEnable = false;
  setInertial(0);
  LBRotation.setPosition(1.33, turns);

  waitTime = 0.5;
  task raise1(lbraise);
  Drive(950, 0.8);
  task::stop(raise1);

  SwerveR(360-40);
  L.stop(hold);
  R.stop(hold);
  waitTime = 0;
  task lbforward1(alliancescore);
  wait(0.5, sec);
  task::stop(lbforward1);
  waitTime = 0.2;
  task lbforward2(lbforward);
  //DoinkerR.set(true);
  Turn(360-(40-120), 0.5);
  LBMotors.spin(forward, 12, volt);
  Turn(360-45);
  DoinkerL.set(false);
  Drive(-500);
  ConveyorMotor.spin(forward, 12, volt);
  wait(0.05, sec);
  ConveyorMotor.stop(coast);
  Clamp.set(true);
  LBMotors.spin(reverse, 12, volt);
  Turn(360-320);
  Drive(600);
  LBMotors.stop();
  DoinkerL.set(true);     // under ladder
  wait(0.1, sec);
  ConveyorMotor.spin(reverse, 12, volt);
  Drive(-1100, 1.5);
  AJEnable = true;
  
  waitTime = 0.3;
  task undoink1(ldoinker);
  Turn(360-30);

  ConveyorMotor.spin(forward, 12, volt);
  
  waitTime = 0;
  task load1(lbload);
  Drive(400);
  Turn(360-41);
  
  AJEnable = false;
  ColorLock = true;
  Drive(200, 0.5);


  /**
  Turn(230);
  Drive(1100);
  ConveyorMotor.stop(coast);



  task score1(alliancescore);
  wait(0.6, sec);
  task::stop(score1);
  LBMotors.stop(hold);
  Drive(-150, 0.4);

  LBMotors.spin(reverse, 12, volt);
  SwerveL(310);
  Drive(100, 0.3);
  DoinkerR.set(true);
  wait(0.1, sec);
  Turn(213);
  Drive(-930);
  /**/


  Turn(360-43);
  Drive(500);
  ConveyorMotor.stop(coast);
  waitTime = 0;
  task lbwall1(lbwall);
  wait(0.3, sec);
  ColorLock = false;
  ConveyorMotor.spin(forward, 12, volt);
}

void autonRedGoalRushSWP()
{
  Brain.Timer.reset();
  SortColor = &blue;
  ColorLock = true;
  AJEnable = false;
  
  setInertial(18.4);

  ConveyorMotor.spin(forward, 12, volt);
  task cstop1(ringstop2);
  waitTime = 0.7;
  task doink1(ldoinkerd);
  Drive(1050, 1.1);
  //DoinkerL.set(true);
  //wait(0.2, sec);
  Drive(-400);
  DoinkerL.set(false);
  wait(0.1, sec);

  Turn(180, 0.6);
  task::stop(cstop1);
  Drive(-250, 0.7);
  Clamp.set(true);
  wait(0.2, sec);
  ConveyorMotor.spin(forward, 12, volt);

  wait(0.4, sec);
  Clamp.set(false);
  Turn(80, 0.7);
  Drive(-550, 0.9);
  Clamp.set(true);
  wait(0.1, sec);
  AJEnable = true;
  ColorLock = false;
  Turn(320);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(560);
  DoinkerR.set(true);
  wait(0.2, sec);
  waitTime = 0.3;
  task cback1(conveyorback);
  Drive(-550);
  Turn(160);
  DoinkerR.set(false);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(700, 1.1);
  Turn(110);
  Drive(800, 1.3);
  SwerveL(135);
  cornerGrab();
  task cstop2(ringstop);
  LBMotors.spin(reverse, 12, volt);
  Drive(-200);
  LBMotors.stop();
  task lbload1(lbload);
  Turn(270);
  ConveyorMotor.spin(forward, 12, volt);
  Drive(700);
  Turn(220);
  Drive(300);
  task score1(alliancescore);

}

void autonBlueGoalRushSWP()
{
  Brain.Timer.reset();
  SortColor = &red;
  ColorLock = true;
  AJEnable = false;
  
  setInertial(360-15);

  ConveyorMotor.spin(forward, 12, volt);
  task cstop1(ringstop);
  Drive(970, 1.1);
  DoinkerR.set(true);
  wait(0.2, sec);
  Drive(-400);
  DoinkerR.set(false);

  Turn(360-180, 0.6);
  task::stop(cstop1);
  Drive(-200, 0.7);
  Clamp.set(true);
  wait(0.2, sec);
  ConveyorMotor.spin(forward, 12, volt);

  wait(0.4, sec);
  Clamp.set(false);
  Turn(360-95, 0.7);
  Drive(-550, 0.9);
  Clamp.set(true);
  wait(0.1, sec);
  AJEnable = true;
  ColorLock = false;
  Turn(360-160);
  Drive(800, 1.1);
  Turn(360-95);
  Drive(900, 1.3);
  Drive(100, 0.6);
  cornerGrab();
  Drive(-200);
  Turn(360-265);
  Drive(900, 1.5);
  DoinkerL.set(true);
  wait(0.2, sec);
  Drive(-300);
  DoinkerL.set(false);
  Turn(360-285);
  task load1(lbload);
  Drive(200);
  Turn(360-180);
  task score1(alliancescore);
}

void autonSkills()
{
  timer timeout1;
  timer timeout2;
  timer timeout3;
  timer timeout4;
  timer timeout5;
  timer timeout6;
  Brain.Timer.reset();
  SortColor = &blue;
  CorrectLock = true;
  ColorLock = true;
  AJEnable = false;
  setInertial(0);

  /**/
  LBMotors.spin(reverse);
  
  ConveyorMotor.spin(forward);
  IntakeMotor.spin(forward);
  wait(0.4, sec);

  Drive(340, 0.9);
  LBMotors.stop();
  AJEnable = true;
  Turn(90, 0.85);
  ConveyorMotor.stop();
  DriveS(-580, 0.8, 0);
  Clamp.set(true);

  /////////////////////////////  SIDE 1  //////////////////////////////////

  AJEnable = true;
  Turn(0);
  ConveyorMotor.spin(forward);
  IntakeMotor.spin(forward);
  //AJEnable = false;
  Drive(520);
  Turn(360-50);
  Drive(1110, 1.2);
  Drive(-330);
  Turn(180);
  AJEnable = true;
  Drive(1020, 1.3, 0.09);                   // triple
  Drive(350, 0.8, 0.07);                  // farther

  ////////////////////////////////  CORNER 1  ////////////////////////////////
  
  LBMotors.stop();

  Turn(190, 0.4);
  Drive(-280, 0.8);
  Turn(360-90, 0.7);
  Drive(300, 0.8);
  Drive(-270);
  Turn(360-315, 0.9);
  AJEnable = false;
  Drive(-620, 0.8);                     // into corner
  ConveyorMotor.spin(reverse);
  wait(0.1, sec);
  Clamp.set(false);


  ////////////////////////////  NEW CLAMP  //////////////////////////

  std::cout << "corner 1 at " << Brain.Timer.value() << "\n";


  Drive(400);                         // out of the corner
  ConveyorMotor.stop();
  IntakeMotor.stop();
  
  Turn(270);
  LBMotors.spin(forward);
  Drive(-1520, 1.3);                 // long clamp 2 drive
  LBMotors.spin(reverse);
  Turn(270, 0.2);
  Drive(-220, 0.7);
  Clamp.set(true);



  ///////////////////////////////  SIDE 2  ///////////////////////////////


  AJEnable = true;
  Turn(0);
  ConveyorMotor.spin(forward);
  IntakeMotor.spin(forward);
  //AJEnable = false;
  Drive(520, 0.8);
  Turn(50, 0.7);
  Drive(1110, 1.2);
  Drive(-330, 0.8);
  Turn(180, 0.7);
  AJEnable = true;
  Drive(1020, 1.3, 0.09);                   // triple
  Drive(350, 0.8, 0.07);                  // farther

  ////////////////////////////////  CORNER 2  ////////////////////////////////
  
  LBMotors.stop();

  Turn(360-190, 0.4);
  Drive(-280, 0.8);
  Turn(90, 0.7);
  Drive(400, 0.8);
  Drive(-300);
  Turn(315, 0.9);
  AJEnable = false;
  Drive(-550, 0.8);
  ConveyorMotor.spin(reverse);
  wait(0.1, sec);
  Clamp.set(false);

  std::cout << "corner 2 at " << Brain.Timer.value() << "\n";

  
  AJEnable = false;

  
  ConveyorMotor.stop();
  IntakeMotor.spin(forward);

  /////////////////////////////////  LADDER  /////////////////////////////////
  Drive(400, 0.9);
  Turn(316, 0.3);
  Drive(1600, 1.5, 0.08);
  ConveyorMotor.spin(forward, 400, rpm);
  wait(0.46, sec);
  ConveyorMotor.stop();

  Turn(315, 0.3);
  waitTime = 0.6;
  //task load3(lbload);
  Drive(900, 1);
  AJEnable = false;


  /////////////////////////////////  ALLIANCE STAKE  /////////////////////////////////

  
  Turn(221, 0.9);
  Drive(-900, 1.1, 0.07);
  
  Clamp.set(true);
  /**
  Drive(50, 0.5, 0.09);

  ConveyorMotor.stop();

  Turn(0.4);
  Drive(550, 0.9, 0.06);

  Drive(100, 0.5, 0.05);


  ConveyorMotor.stop();
  IntakeMotor.spin(forward);
  Clamp.set(true);
  Drive(-170);

  std::cout << "alliance stake at " << Brain.Timer.value() << "\n";


  float sp4 = ConveyorMotor.position(degrees);
  timeout4.reset();
  ConveyorMotor.spin(reverse, 200, rpm);
  waitUntil(ConveyorMotor.position(degrees) < sp4 - 55 || timeout4.value() > 0.4);
  ConveyorMotor.stop(hold);
  wait(0.05, sec);
  LBMotorL.spin(forward);
  wait(0.8, sec);

  ConveyorMotor.spin(forward);

  LBMotorL.spin(reverse);
  Drive(-200);
  /**/

  /////////////////////////////////  3RD SIDE  /////////////////////////////////

  AJEnable = true;
  Turn(132, 0.7);
  ConveyorMotor.spin(forward);
  Drive(900);
  Turn(90);
  Drive(600);
  Turn(27, 0.7);
  ColorLock = false;
  Drive(590, 1, 0.06);
  Drive(-400, 0.7);
  Turn(358, 0.6);
  Drive(520, 0.8);
  Turn(228, 0.7);
  Clamp.set(false);
  Drive(-400, 0.8);
  Drive(50);
  Drive(-100);
  ConveyorMotor.spin(reverse);
  wait(0.1, sec);
  AJEnable = false;

  ////////////////////////////////  PUSH GOAL  ////////////////////////////////
  Drive(600);
  Turn(110);
  Drive(-1800, 1.5);
  Drive(-630, 1);
  Turn(145);

  Drive(50);
  Drive(-150);
  ConveyorMotor.spin(forward);
  IntakeMotor.spin(forward);

  Drive(1130);
  LBMotors.spin(forward);
  Turn(315);
  ConveyorMotor.stop(coast);
  IntakeMotor.stop();

  ////////////////////////////////  HANG  /////////////////////////////////

  Drive(-600, 1.4, 0.067);
  LBMotors.spin(reverse);
  Drive(400, 0.5, 0.058);
  //wait(0.1, sec);
  //Drive(400, 0.4, 0.07);


  std::cout << "end at " << Brain.Timer.value() << "\n";
}
