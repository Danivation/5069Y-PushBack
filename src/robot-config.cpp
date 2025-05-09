#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// VEXcode device constructors
brain Brain;
controller Controller  (primary);

motor LA                (PORT11, ratio6_1, true);
motor LB                (PORT12, ratio6_1, true);
motor LC                (PORT13, ratio6_1, true);
motor RA                (PORT14, ratio6_1, false);
motor RB                (PORT15, ratio6_1, false);
motor RC                (PORT16, ratio6_1, false);

motor_group L           (LA, LB, LC);
motor_group R           (RA, RB, RC);

motor LBMotorL          (PORT17, ratio18_1, true);
motor LBMotorR          (PORT18, ratio18_1, false);
rotation LBRotation     (PORT19, false);
motor_group LBMotors    (LBMotorL, LBMotorR);
motor ConveyorMotor     (PORT20, ratio6_1, true);

rotation HTracker       (PORT1, false);    // CORRECT
rotation VTracker       (PORT2, false);    // CORRECT
inertial Inertial1      (PORT3);
inertial Inertial2      (PORT4);

optical RingColor       (PORT10);
distance RingDistance   (PORT9);


digital_out Clamp       (Brain.ThreeWirePort.G);
digital_out DoinkerL    (Brain.ThreeWirePort.H);
digital_out DoinkerR    (Brain.ThreeWirePort.C);
digital_in LBLimit      (Brain.ThreeWirePort.D);



// unused ports
motor IntakeMotor       (PORT5, ratio18_1, false);
aivision FrontVision (PORT5);

/*vex-vision-config:begin*
signature OldFrontVision__RING_RED(1, 9671, 12289, 10980, -1047, -255, -651, 6, 0);
signature OldFrontVision__RING_BLUE(2, -3629, -3197, -3413, 8977, 9929, 9453, 9.8, 0);
signature OldFrontVision__MOGO_YELLOW(3, -1239, -587, -913, -5645, -4943, -5294, 8.3, 0);
vision OldFrontVision(PORT10, 50, OldFrontVision__RING_RED, OldFrontVision__RING_BLUE, OldFrontVision__MOGO_YELLOW);
/*vex-vision-config:end*/
//aivision FrontVision(PORT10, aivision::ALL_AIOBJS);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}