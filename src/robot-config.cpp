#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// VEXcode device constructors
brain Brain;
controller Controller (primary);

motor LA              (PORT11, ratio6_1, true);
motor LB              (PORT12, ratio6_1, true);
motor LC              (PORT13, ratio6_1, true);
motor RA              (PORT14, ratio6_1, false);
motor RB              (PORT15, ratio6_1, false);
motor RC              (PORT16, ratio6_1, false);

motor_group L         (LA, LB, LC);
motor_group R         (RA, RB, RC);

rotation HTracker     (PORT1, false);
rotation VTracker     (PORT2, false);
inertial Inertial1    (PORT3);
inertial Inertial2    (PORT4);

odometry::tracker c_horizontal  (&HTracker, 0, 2.75);
odometry::tracker c_vertical    (&VTracker, 0, 2.75);
odometry Odometry               (&c_horizontal, &c_vertical);


motor IntakeL         (PORT5, ratio18_1, false);
motor IntakeR         (PORT6, ratio18_1, true);
motor_group Intake    (IntakeL, IntakeR);
motor Conveyor        (PORT7, ratio6_1, false);

digital_out Lift      (Brain.ThreeWirePort.A);

optical BlockColor    (PORT8);

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