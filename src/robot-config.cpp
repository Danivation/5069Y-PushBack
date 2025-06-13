#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// VEXcode device constructors
brain Brain;
controller Controller (primary);

/**/
motor LA              (PORT1, ratio6_1, false);
motor LB              (PORT2, ratio6_1, false);
motor LC              (PORT3, ratio6_1, true);
motor RA              (PORT4, ratio6_1, true);
motor RB              (PORT5, ratio6_1, true);
motor RC              (PORT6, ratio6_1, false);
/**
motor LA              (PORT7, ratio6_1, true );
motor LB              (PORT9, ratio6_1, false);
motor LC              (PORT8, ratio6_1, true );
motor RA              (PORT1, ratio6_1, false);
motor RB              (PORT2, ratio6_1, true );
motor RC              (PORT3, ratio6_1, false);
/**/

motor_group L         (LA, LB, LC);
motor_group R         (RA, RB, RC);

motor Intake1         (PORT7, ratio6_1, true);
motor Intake2         (PORT8, ratio6_1, true);
motor_group Intake    (Intake1, Intake2);

digital_out Lift      (Brain.ThreeWirePort.A);
digital_out Trapdoor  (Brain.ThreeWirePort.B);
digital_out Stopper   (Brain.ThreeWirePort.C);
digital_out Loader    (Brain.ThreeWirePort.D);
digital_out Parker    (Brain.ThreeWirePort.E);

optical BlockColor    (PORT20);

inertial Inertial1    (PORT9);
inertial Inertial2    (PORT20);
rotation VTracker     (PORT10, false);
rotation HTracker     (PORT19, false);


odometry::tracker c_horizontal  (&HTracker, 4.31, 2.75);        // positive is back
odometry::tracker c_vertical    (&VTracker, 1.22, 2.75);        // positive is left
odometry Odometry               (&c_horizontal, &c_vertical);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
void vexcodeInit(void) {
  // nothing to initialize
}