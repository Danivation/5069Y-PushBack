#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// VEXcode device constructors
brain Brain;
controller Controller (primary);

motor LA              (PORT7, ratio6_1, true );
motor LB              (PORT9, ratio6_1, false);
motor LC              (PORT8, ratio6_1, true );
motor RA              (PORT1, ratio6_1, false);
motor RB              (PORT2, ratio6_1, true );
motor RC              (PORT3, ratio6_1, false);

motor_group L         (LA, LB, LC);
motor_group R         (RA, RB, RC);

rotation HTracker     (PORT11, false);
rotation VTracker     (PORT12, false);
inertial Inertial1    (PORT19);
inertial Inertial2    (PORT20);

odometry::tracker c_horizontal  (&HTracker, 5.48, 2.75);
odometry::tracker c_vertical    (&VTracker, -0.08, 2.75);
odometry Odometry               (&c_horizontal, &c_vertical);


motor Intake1         (PORT15, ratio18_1, false);
motor Intake2         (PORT16, ratio18_1, true);
motor_group Intake    (Intake1, Intake2);
motor Conveyor        (PORT17, ratio6_1, false);

digital_out Lift      (Brain.ThreeWirePort.A);
digital_out Trapdoor  (Brain.ThreeWirePort.B);
digital_out Loader    (Brain.ThreeWirePort.C);
digital_out Stopper   (Brain.ThreeWirePort.D);
digital_out Parker    (Brain.ThreeWirePort.E);

optical BlockColor    (PORT18);

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