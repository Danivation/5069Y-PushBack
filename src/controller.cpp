#include "vex.h"
using namespace vex;

// lower number for higher load position
// higher number for lower load position
const float LoadPosition = 1.32;
const float DescorePosition = 2.37;     // descore really (A button)
const float Descore2Position = 2.44;   // alliance stake (X button)

float LoadKP = 150;
float IntStart = 0.1;
float LoadKI = 0;

// Controller bindings
extern const vex::controller::axis   ForwardAxis        = Controller1.Axis3;
extern const vex::controller::axis   TurnAxis           = Controller1.Axis1;

extern const vex::controller::button IntakeInButton     = Controller1.ButtonR1;
extern const vex::controller::button IntakeOutButton    = Controller1.ButtonR2;

extern const vex::controller::button ClampToggle        = Controller1.ButtonDown;
extern const vex::controller::button DoinkerLToggle     = Controller1.ButtonRight;
extern const vex::controller::button DoinkerRToggle     = Controller1.ButtonY;

extern const vex::controller::button LBUpButton         = Controller1.ButtonL1;
extern const vex::controller::button LBDownButton       = Controller1.ButtonL2;
extern const vex::controller::button LBLoadButton       = Controller1.ButtonB;
extern const vex::controller::button LBAllianceButton   = Controller1.ButtonA;

extern const vex::controller::button LBDescore2Button  = Controller1.ButtonX;

int selectedAuton = 0;
int autonMode = 101;
std::string autonName = "";
std::string autonModeName = "";
std::string finalAutoName = "";
bool Bypass = false;

// Control
int AutonSelect()
{
  // brain screen size is 480x240
  int h = 80;
  int w = 80;

  int mH = 105;

  // boxes
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawRectangle(240-w, mH-h, w, h);
  Brain.Screen.drawRectangle(240,   mH-h, w, h);
  Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(240-w, mH,   w, h);
  Brain.Screen.drawRectangle(240,   mH,   w, h);

  // extra boxes
  Brain.Screen.setFillColor(black);
  Brain.Screen.drawRectangle(240-w/2-170, 125-h/2, w, h);
  Brain.Screen.drawRectangle(240-w/2+170, 125-h/2, w, h);
  Brain.Screen.drawRectangle(240-200/2, 200, 200, 35);    // confirm

  int sh = 50;
  int sw = 50;
  Brain.Screen.setPenWidth(8);
  Brain.Screen.setFont(vex::fontType::mono30);

  // symbols
  Brain.Screen.drawLine(240-w/2-sw/2, mH-h/2,      240-w/2+sw/2, mH-h/2);
  Brain.Screen.drawLine(240-w/2-sw/2, mH+h/2,      240-w/2+sw/2, mH+h/2);
  Brain.Screen.drawLine(240+w/2-sw/2, mH-h/2,      240+w/2+sw/2, mH-h/2);
  Brain.Screen.drawLine(240+w/2-sw/2, mH+h/2,      240+w/2+sw/2, mH+h/2);
  Brain.Screen.drawLine(240+w/2,      mH-h/2-sh/2, 240+w/2,      mH-h/2+sh/2);
  Brain.Screen.drawLine(240+w/2,      mH+h/2-sh/2, 240+w/2,      mH+h/2+sh/2);

  // text
  Brain.Screen.printAt(240-(Brain.Screen.getStringWidth("None")/2)-170, 125+(Brain.Screen.getStringHeight("None")/4), "None");
  Brain.Screen.printAt(240-(Brain.Screen.getStringWidth("Skills")/2)+170, 125+(Brain.Screen.getStringHeight("Skills")/4), "Skills");
  Brain.Screen.setFont(vex::fontType::mono30);
  Brain.Screen.printAt(240-(Brain.Screen.getStringWidth("Confirm")/2), 220+(Brain.Screen.getStringHeight("Confirm")/4), "Confirm");


  // response
  Brain.Screen.setFont(vex::fontType::mono20);
  Brain.Screen.setFillColor(black);
  Brain.Screen.setPenWidth(1);
  while (true)
  {
    int lastX = Brain.Screen.xPosition();
    int lastY = Brain.Screen.yPosition();
    std::string prevName = autonName;

    // none
    if (lastX > 240-170-h/2 && lastX < 240-170+h/2 && lastY < mH+h/2 && lastY > mH-h/2) {selectedAuton = 101; autonName = "None"; Bypass = true; break;}
    // blue ring
    if (lastX > 240-h && lastX < 240 && lastY > mH-h && lastY < mH) {selectedAuton = 0; autonName = "Blue Ring";}
    // blue goal
    if (lastX > 240 && lastX < 240+h && lastY > mH-h && lastY < mH) {selectedAuton = 3; autonName = "Blue Goal";}
    // red ring
    if (lastX > 240-h && lastX < 240 && lastY < mH+h && lastY > mH) {selectedAuton = 6; autonName = "Red Ring";}
    // red goal
    if (lastX > 240 && lastX < 240+h && lastY < mH+h && lastY > mH) {selectedAuton = 9; autonName = "Red Goal";}
    // skills
    if (lastX > 240+170-h/2 && lastX < 240+170+h/2 && lastY < mH+h/2 && lastY > mH-h/2) {selectedAuton = 100; autonName = "Skills"; break;}

    if (prevName != autonName)
    {
      Brain.Screen.setPenWidth(1);
      Brain.Screen.setFillColor(blue);
      Brain.Screen.drawRectangle(240-w, mH-h, w, h);
      Brain.Screen.drawRectangle(240,   mH-h, w, h);
      Brain.Screen.setFillColor(red);
      Brain.Screen.drawRectangle(240-w, mH,   w, h);
      Brain.Screen.drawRectangle(240,   mH,   w, h);
      Brain.Screen.setFillColor(green);
      if (selectedAuton == 0) { Brain.Screen.drawRectangle(240-h, mH-h, w, h); }
      if (selectedAuton == 3) { Brain.Screen.drawRectangle(240, mH-h, w, h); }
      if (selectedAuton == 6) { Brain.Screen.drawRectangle(240-h, mH, w, h); }
      if (selectedAuton == 9) { Brain.Screen.drawRectangle(240, mH, w, h); }

      Brain.Screen.setPenColor(white);
      // symbols
      Brain.Screen.setPenWidth(8);
      Brain.Screen.drawLine(240-w/2-sw/2, mH-h/2,      240-w/2+sw/2, mH-h/2);
      Brain.Screen.drawLine(240-w/2-sw/2, mH+h/2,      240-w/2+sw/2, mH+h/2);
      Brain.Screen.drawLine(240+w/2-sw/2, mH-h/2,      240+w/2+sw/2, mH-h/2);
      Brain.Screen.drawLine(240+w/2-sw/2, mH+h/2,      240+w/2+sw/2, mH+h/2);
      Brain.Screen.drawLine(240+w/2,      mH-h/2-sh/2, 240+w/2,      mH-h/2+sh/2);
      Brain.Screen.drawLine(240+w/2,      mH+h/2-sh/2, 240+w/2,      mH+h/2+sh/2);

      // text
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
      Brain.Screen.drawRectangle(240-150, 0, 390, 22);
      Brain.Screen.setPenColor(white);
      Brain.Screen.printAt(240-(Brain.Screen.getStringWidth(autonName.c_str())/2), 18, autonName.c_str());
    }

    if (lastX > 240-200/2 && lastX < 240+200/2 && lastY > 200 && lastY < 235 && Brain.Screen.pressing()) {break;}
    wait(10, msec);
  }

  // new stuff
  Brain.Screen.clearScreen();
  printf("idfsjdf\n");
  wait(500, msec);

  // quals basic wp all 4 (1+4)
  // quals ring swoop solo wp (1+4+1)
  // elims ring swoop (1+5)
  // elims goal rush (1+1+1)
  Brain.Screen.setPenWidth(1);
  Brain.Screen.setPenColor(white);
  Brain.Screen.setFillColor(black);
  Brain.Screen.drawRectangle(60, 70, 100, 100);
  Brain.Screen.drawRectangle(190, 70, 100, 100);
  Brain.Screen.drawRectangle(320, 70, 100, 100);

  // ring side
  if (selectedAuton == 0 || selectedAuton == 6)
  {
    Brain.Screen.printAt(60+50-(Brain.Screen.getStringWidth("Basic WP")/2), 110, "Basic WP");
    Brain.Screen.printAt(190+50-(Brain.Screen.getStringWidth("Swoop SWP")/2), 110, "Swoop SWP");
    Brain.Screen.printAt(320+50-(Brain.Screen.getStringWidth("Swoop Elims")/2), 110, "Swoop Elims");
  }
  // goal side
  if (selectedAuton == 3 || selectedAuton == 9)
  {
    Brain.Screen.printAt(60+50-(Brain.Screen.getStringWidth("Basic WP")/2), 110, "Basic WP");
    Brain.Screen.printAt(190+50-(Brain.Screen.getStringWidth("RING corner WP")/2), 110, "RING corner WP");
    Brain.Screen.printAt(320+50-(Brain.Screen.getStringWidth("Rush Elims")/2), 110, "Rush Elims");
  }


  int cw = 150;
  Brain.Screen.setFillColor(black);
  Brain.Screen.drawRectangle(240-200/2, 200, 200, 35);    // confirm
  Brain.Screen.printAt(240-(Brain.Screen.getStringWidth("Confirm & Calibrate")/2), 220+(Brain.Screen.getStringHeight("Confirm & Calibrate")/4), "Confirm & Calibrate");
  while (true)
  {
    int lastX = Brain.Screen.xPosition();
    int lastY = Brain.Screen.yPosition();
    std::string prevName = autonModeName;

    if (lastX > 60 && lastX < 160 && lastY > 70 && lastY < 170) {autonMode = 1; autonModeName = "Basic WP (1+4)";}
    if (lastX > 190 && lastX < 290 && lastY > 70 && lastY < 170 && (selectedAuton == 0 || selectedAuton == 6)) {autonMode = 2; autonModeName = "Swoop SWP (1+4+1)";}
    if (lastX > 190 && lastX < 290 && lastY > 70 && lastY < 170 && (selectedAuton == 3 || selectedAuton == 9)) {autonMode = 2; autonModeName = "RING SIDE Swoop Corner WP (1+4)";}
    if (lastX > 320 && lastX < 420 && lastY > 70 && lastY < 170 && (selectedAuton == 0 || selectedAuton == 6)) {autonMode = 3; autonModeName = "Swoop Elims (1+5)";}
    if (lastX > 320 && lastX < 420 && lastY > 70 && lastY < 170 && (selectedAuton == 3 || selectedAuton == 9)) {autonMode = 3; autonModeName = "Rush Elims (1+1+1)";}

    if (prevName != autonModeName)
    {
      Brain.Screen.setFillColor(black);
      Brain.Screen.drawRectangle(60, 70, 100, 100);
      Brain.Screen.drawRectangle(190, 70, 100, 100);
      Brain.Screen.drawRectangle(320, 70, 100, 100);
      Brain.Screen.setFillColor(green);
      if (autonMode == 1) { Brain.Screen.drawRectangle(60, 70, 100, 100); }
      if (autonMode == 2) { Brain.Screen.drawRectangle(190, 70, 100, 100); }
      if (autonMode == 3) { Brain.Screen.drawRectangle(320, 70, 100, 100); }
      // ring side
      Brain.Screen.setFillColor(black);
      if (selectedAuton == 0 || selectedAuton == 6)
      {
        Brain.Screen.printAt(60+50-(Brain.Screen.getStringWidth("Basic WP")/2), 110, "Basic WP");
        Brain.Screen.printAt(190+50-(Brain.Screen.getStringWidth("Swoop SWP")/2), 110, "Swoop SWP");
        Brain.Screen.printAt(320+50-(Brain.Screen.getStringWidth("Swoop Elims")/2), 110, "Swoop Elims");
      }
      // goal side
      if (selectedAuton == 3 || selectedAuton == 9)
      {
        Brain.Screen.printAt(60+50-(Brain.Screen.getStringWidth("Basic WP")/2), 110, "Basic WP");
        Brain.Screen.printAt(190+50-(Brain.Screen.getStringWidth("RING corner WP")/2), 110, "RING corner WP");
        Brain.Screen.printAt(320+50-(Brain.Screen.getStringWidth("Rush Elims")/2), 110, "Rush Elims");
      }

      printf("Auton: %s %s\n", autonName.c_str(), autonModeName.c_str());
      Brain.Screen.setFillColor(black);
      Brain.Screen.setPenColor(black);
      Brain.Screen.drawRectangle(0, 0, 480, 22);
      Brain.Screen.setPenColor(white);
      finalAutoName = autonName + " " + autonModeName;
      Brain.Screen.printAt(240-(Brain.Screen.getStringWidth(finalAutoName.c_str())/2), 18, finalAutoName.c_str());
    }

    // if confirm
    if (lastX > 240-200/2 && lastX < 240+200/2 && lastY > 200 && lastY < 235 && Brain.Screen.pressing())
    {
      Brain.Screen.clearScreen();
      Inertial.calibrate();
      Inertial2.calibrate();
      Brain.Screen.setFillColor(orange);
      Brain.Screen.setPenColor(black);
      Brain.Screen.drawRectangle(-10, -10, 500, 260);
      Brain.Screen.printAt(240-(Brain.Screen.getStringWidth(finalAutoName.c_str())/2), 20, finalAutoName.c_str());
      wait(3, sec);
      setInertial(0);
      Controller1.rumble(rumbleShort);
      break;
    }
    wait(10, msec);
  }
  while (true)
  {
    if (trueHeading() > 359.5 || trueHeading() < 0.5) {Brain.Screen.setFillColor(green); Brain.Screen.setPenColor(black);}
    else {Brain.Screen.setFillColor(red); Brain.Screen.setPenColor(white);}
    Brain.Screen.drawRectangle(-10, -10, 500, 260);
    Brain.Screen.printAt(240-(Brain.Screen.getStringWidth(finalAutoName.c_str())/2), 20, finalAutoName.c_str());
    wait(50, msec);
  }
  return 1;
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

int DrivetrainControl()
{
  float throttle;
  float turn;
  while (true)
  {
    if (!false)
    {
      throttle = DeadBand(ForwardAxis.value(), 5);
      turn = DeadBand(TurnAxis.value(), 5);
      L.spin(forward, ToVolt(throttle + turn), volt);
      R.spin(forward, ToVolt(throttle - turn), volt);
      wait(5, msec);
    }
  }
}

int ConveyorControl()
{
  while (true)
  {
    if (IntakeInButton.pressing())
    {
      ConveyorMotor.spin(forward, ToVolt(100), volt);
      waitUntil(!IntakeInButton.pressing());
      ConveyorMotor.stop();
    }
    if (IntakeOutButton.pressing())
    {
      ConveyorMotor.spin(reverse, ToVolt(100), volt);
      waitUntil(!IntakeOutButton.pressing());
      ConveyorMotor.stop();
    }
  }
}

int ClampToggleControl()
{
  bool MogoOState = !Clamp.value();
  //bool MogoIState = !MogoIn.value();
  while (true)
  {
    Clamp.set(!MogoOState);
    //MogoIn.set(MogoOState);
    waitUntil(!ClampToggle.pressing());
    waitUntil(ClampToggle.pressing());
    MogoOState = Clamp.value();
    //MogoIState = MogoIn.value();
  }
}

int DoinkerLControl()
{
  bool DoinkerLState = true;
  while (true)
  {
    DoinkerL.set(!DoinkerLState);
    waitUntil(!DoinkerLToggle.pressing());
    waitUntil(DoinkerLToggle.pressing());
    DoinkerLState = !DoinkerLState;
  }
}

int DoinkerRControl()
{
  bool DoinkerRState = true;
  while (true)
  {
    DoinkerR.set(!DoinkerRState);
    waitUntil(!DoinkerRToggle.pressing());
    waitUntil(DoinkerRToggle.pressing());
    DoinkerRState = !DoinkerRState;
  }
}

bool Loading = false;
int LBControl()
{
  while (true)
  {
    if (LBDownButton.pressing())
    {
      task::stop(load);
      task::stop(descore);
      task::stop(descore2);
      Loading = false;
      LBMotorL.spin(reverse, ToVolt(100), volt);
      LBMotorR.spin(reverse, ToVolt(100), volt);
      waitUntil(!LBDownButton.pressing());
      LBMotorL.stop(hold);
      LBMotorR.stop(hold);
    }
    if (LBUpButton.pressing())
    {
      task::stop(load);
      task::stop(descore);
      task::stop(descore2);
      Loading = false;
      LBMotorL.spin(forward, ToVolt(100), volt);
      LBMotorR.spin(forward, ToVolt(100), volt);
      waitUntil(!LBUpButton.pressing());
      LBMotorL.stop(hold);
      LBMotorR.stop(hold);
    }
    if (!Loading)
    {
      LBMotorL.stop(hold);
      LBMotorR.stop(hold);
    }
  }
}

int LBReset()
{
  bool PValue = false;
  while (true)
  {
    if (LBLimit.value() == false)
    {
      LBRotation.setPosition(1, turns);
    }
    PValue = LBLimit.value();
    wait(5, msec);
  }
}

int LBLoadControl()
{
  while (true)
  {
    waitUntil(!LBLoadButton.pressing());
    waitUntil(LBLoadButton.pressing());
    Loading = true;
    task lbload1(load);
    wait(0.7, sec);
    task::stop(lbload1);
    //LBMotor.stop();
    Loading = false;
  }
}

int LBDescoreControl()
{
  while (true)
  {
    waitUntil(!LBAllianceButton.pressing());
    waitUntil(LBAllianceButton.pressing());
    Loading = true;
    task descore1(descore);
    wait(0.7, sec);
    task::stop(descore1);
    //LBMotor.stop();
    Loading = false;
  }
}

int LBDescore2Control()
{
  while (true)
  {
    waitUntil(!LBDescore2Button.pressing());
    waitUntil(LBDescore2Button.pressing());
    Loading = true;
    task descore1(descore2);
    wait(0.7, sec);
    task::stop(descore1);
    //LBMotor.stop();
    Loading = false;
  }
}

int load()
{
  float error;
  float integral;
  float startint = IntStart;
  float kp = LoadKP;
  float ki = LoadKI;
  while (!LBUpButton.pressing() && !LBDownButton.pressing() && !LBAllianceButton.pressing())
  {
    if (!LBUpButton.pressing() && !LBDownButton.pressing() && !LBAllianceButton.pressing())
    {
      error = (LoadPosition - LBRotation.position(turns));  
      //Brain.Screen.printAt(0, 100, "%f", error);
      //Brain.Screen.printAt(0, 150, "%f", error*kp);
      if (fabs(error) < startint)
      {
        integral += error;
      }
      if ((Dist>0 && PDist<0)||(Dist<0 && PDist>0))
      {
        integral = 0;
      }
      LBMotorL.spin(forward, ToVolt(error*kp + integral*ki), volt); 
      LBMotorR.spin(forward, ToVolt(error*kp + integral*ki), volt); 
      wait(15, msec);
    }
    else
    {
      LBMotorL.stop(hold);
      LBMotorR.stop(hold);
      break;
    }
  }
  return 1;
}

int descore()
{
  float error;
  float kp = LoadKP;
  while (!LBUpButton.pressing() && !LBDownButton.pressing() && !LBLoadButton.pressing())
  {
    if (!LBUpButton.pressing() && !LBDownButton.pressing() && !LBLoadButton.pressing())
    {
      error = (DescorePosition - LBRotation.position(turns));  
      //Brain.Screen.printAt(0, 100, "%f", error);
      //Brain.Screen.printAt(0, 150, "%f", error*kp);
      LBMotorL.spin(forward, ToVolt(error*kp), volt); 
      LBMotorR.spin(forward, ToVolt(error*kp), volt); 
      wait(15, msec);
    }
    else
    {
      LBMotorL.stop(hold);
      LBMotorR.stop(hold);
      break;
    }
  }
  return 1;
}

int descore2()
{
  float error;
  float kp = LoadKP;
  while (!LBUpButton.pressing() && !LBDownButton.pressing() && !LBLoadButton.pressing())
  {
    if (!LBUpButton.pressing() && !LBDownButton.pressing() && !LBLoadButton.pressing())
    {
      error = (Descore2Position - LBRotation.position(turns));  
      //Brain.Screen.printAt(0, 100, "%f", error);
      //Brain.Screen.printAt(0, 150, "%f", error*kp);
      LBMotorL.spin(forward, ToVolt(error*kp), volt); 
      LBMotorR.spin(forward, ToVolt(error*kp), volt); 
      wait(15, msec);
    }
    else
    {
      LBMotorL.stop(hold);
      LBMotorR.stop(hold);
      break;
    }
  }
  return 1;
}
